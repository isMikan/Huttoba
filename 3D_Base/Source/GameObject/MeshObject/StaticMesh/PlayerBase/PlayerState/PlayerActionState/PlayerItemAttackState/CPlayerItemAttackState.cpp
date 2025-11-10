#include "CPlayerItemAttackState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerHoldingIdleState/CPlayerHoldingIdleState.h"

#include "Item/Items/Fun/Fun.h"

#include "Input/CInputManager.h"

CPlayerItemAttackState::CPlayerItemAttackState(CPlayerBase& pPlayer)
	: CPlayerState						( pPlayer )
	
	, m_StartTime						()
	, m_EndTime							( 0.1f )

	, m_CurrentTiltAngle				()
	, m_TiltAngleMax					( D3DXToRadian( 7.f ) )

	, m_RightHandStartPos				()
	, m_LeftHandStartPos				()
	, m_HoldBothHands_RightHandEndPos	( 0.f, 0.3f, 0.3f )
	, m_HoldBothHands_LeftHandEndPos	( 0.f, 0.3f, 0.3f )

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

	//クォータニオンの回転を計算して設定する.
	m_pPlayer.SetQuaternion(m_pPlayer.TiltedQuat(m_StartQuat, axes.right, m_CurrentTiltAngle));

	float eased = sinf(progress * D3DX_PI * 0.5f);	//0.5で半往復させ前に手を出す計算をする.	

	//右手と左手の調整位置だけの計算.
	D3DXVECTOR3 rightHandOffsetPos;
	D3DXVec3Lerp(&rightHandOffsetPos, &m_RightHandStartPos, &m_HoldBothHands_RightHandEndPos, eased);
	D3DXVECTOR3 leftHandOffsetPos;
	D3DXVec3Lerp(&leftHandOffsetPos, &m_LeftHandStartPos, &m_HoldBothHands_LeftHandEndPos, eased);

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(rightHandOffsetPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(leftHandOffsetPos));
}

//--- 入力を受け付けるか判断する ---.
bool CPlayerItemAttackState::IsInput(int index) const
{
	if (dynamic_cast<CPlayer*>(&m_pPlayer))
	{
		return CInputManager::IsUp(Action::Attack, index);
	}
}
