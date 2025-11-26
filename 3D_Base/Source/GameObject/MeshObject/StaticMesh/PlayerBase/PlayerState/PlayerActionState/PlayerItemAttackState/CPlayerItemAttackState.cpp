#include "CPlayerItemAttackState.h"

#include "PlayerBase/CPlayerBase.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHoldingIdleState/CPlayerHoldingIdleState.h"

#include "Item/Items/Haetataki/Haetataki.h"
#include "Item/Items/SmashBat/SmashBat.h"
#include "Item/Items/Bomb/Bomb.h"
#include "Item/Items/Mushroom/Mushroom.h"
#include "Item/Items/Fun/Fun.h"
#include "Item/Items/Boomerang/Boomerang.h"
#include "Item/Items/TrackingRobot/TrackingRobot.h"

#include "Input/CInputManager.h"

CPlayerItemAttackState::CPlayerItemAttackState(CPlayerBase& pPlayer)
	: CPlayerState						( pPlayer )
	
	, m_StartTime						()
	, m_EndTime							( 0.2f )

	, m_CurrentTiltAngle				()
	, m_TiltAngleMax					( D3DXToRadian( 7.f ) )
	, m_HandLaps						( 0.5f )	//半周.

	, m_RightHandStartPos				()
	, m_LeftHandStartPos				()
	, m_RightHandEndPos					()
	, m_LeftHandEndPos					()

	, m_HoldBothHands_RightHandEndPos	( 0.f, 0.3f, 0.3f )
	, m_HoldBothHands_LeftHandEndPos	( 0.f, 0.3f, 0.3f )
	, m_OneHand_RightHandEndPos			( -0.2f, 0.3f, 0.3f )
	, m_OneHand_LeftHandEndPos			( -0.1f, 0.2f, -0.3f )

	, m_StartQuat						( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerItemAttackState::~CPlayerItemAttackState()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerItemAttackState::Enter()
{
	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetTotalTime();

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();
	m_StartQuat = m_pPlayer.GetQuaternion();

	//ローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = m_pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//手の開始位置を設定.
	m_RightHandStartPos = rightHandOffset;
	m_LeftHandStartPos = leftHandOffset;

	ItemBase* item = m_pPlayer.GetItemBase();

	if (dynamic_cast<Haetataki*>(item) 
		|| dynamic_cast<SmashBat*>(item)
		|| dynamic_cast<Boomerang*>(item))
	{
		m_RightHandEndPos = m_OneHand_RightHandEndPos;
		m_LeftHandEndPos = m_OneHand_LeftHandEndPos;
	}
	else if (dynamic_cast<Bomb*>(item)
		|| dynamic_cast<Mushroom*>(item)
		|| dynamic_cast<Fun*>(item)
		|| dynamic_cast<TrackingRobot*>(item))
	{
		m_RightHandEndPos = m_HoldBothHands_RightHandEndPos;
		m_LeftHandEndPos = m_HoldBothHands_LeftHandEndPos;
	}
}

//--- 状態の終了時に呼び出す ---.
void CPlayerItemAttackState::Exit()
{
}

//--- この状態の間に呼び出す ---.
void CPlayerItemAttackState::Update()
{
	//ゲーム全体の経過時間.
	float t = CTimeManager::GetTotalTime();

	ItemBase* item = m_pPlayer.GetItemBase();
		
	if (dynamic_cast<Fun*>(item))
	{
		//プレイヤーの入力を受けた場合.
		if (IsInput(m_pPlayer.GetPlayerID()))
		{
			m_pPlayer.SetActionState(std::make_unique<CPlayerHoldingIdleState>(m_pPlayer));
			return;
		}
	}
	else
	{
		//現在の経過時間と開始時間の差が終了時間を上回った場合.
		if (t - m_StartTime > m_EndTime)
		{
			m_pPlayer.SetActionState(std::make_unique<CPlayerHoldingIdleState>(m_pPlayer));
			return;
		}
	}

	//ローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = std::clamp(progress, 0.f, 1.f);

	//現在の傾き = 最大傾き角度 * 割合.
	m_CurrentTiltAngle = m_pPlayer.WrapAngle(m_TiltAngleMax * progress);

	//クォータニオンの傾く回転を計算する.
	D3DXQUATERNION tiltedQuat = m_pPlayer.TiltedQuat(m_StartQuat, axes.right, m_CurrentTiltAngle);
	//現在のクォータニオンを受け取り他の状態の回転も取り入れる.
	D3DXQUATERNION currentQuat = m_pPlayer.GetQuaternion();
	//クォータニオンを設定する.
	m_pPlayer.SetQuaternion(tiltedQuat.x, currentQuat.y, tiltedQuat.z, currentQuat.w);

	float eased = sinf(progress * D3DX_PI * m_HandLaps);	//補正を計算する.	

	//右手と左手の調整位置
	D3DXVECTOR3 rightHandOffsetPos;
	D3DXVECTOR3 leftHandOffsetPos;

	//手の軌道の計算.
	D3DXVec3Lerp(&rightHandOffsetPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	D3DXVec3Lerp(&leftHandOffsetPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(rightHandOffsetPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(leftHandOffsetPos));
}

//--- 入力を受け付けたか判断する ---.
bool CPlayerItemAttackState::IsInput(int index) const
{
	if (dynamic_cast<CPlayer*>(&m_pPlayer))
	{
		//ボタンを離した場合.
		return CInputManager::IsUp(Action::Attack, index);
	}

	return false;
}
