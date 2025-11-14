#include "CPlayerHoldingIdleState.h"

#include "PlayerBase/CPlayerBase.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

#include "Item/Items/Haetataki/Haetataki.h"
#include "Item/Items/SmashBat/SmashBat.h"
#include "Item/Items/Bomb/Bomb.h"
#include "Item/Items/Mushroom/Mushroom.h"
#include "Item/Items/Fun/Fun.h"
#include "Item/Items/TrackingRobot/TrackingRobot.h"

CPlayerHoldingIdleState::CPlayerHoldingIdleState(CPlayerBase& pPlayer)
	: CPlayerState						( pPlayer )

	, m_StartTime						()
	, m_EndTime							( 2.f )

	, m_HoldBothHands_RightHandEndPos	( -0.2f, 0.f, 0.3f )
	, m_HoldBothHands_LeftHandEndPos	( 0.2f, 0.f, 0.3f )
	, m_OneHand_RightHandEndPos			( 0.f, 0.3f, 0.2f )
	, m_OneHand_LeftHandEndPos			( 0.1f, 0.f, -0.2f )
{
}

CPlayerHoldingIdleState::~CPlayerHoldingIdleState()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerHoldingIdleState::Enter()
{
	ItemBase* item = m_pPlayer.GetItemBase();

	if (item)
	{
		item->SetState(ItemBase::State::Have);
	}

	//アニメーション開始時間.
	m_StartTime = CTimeManager::GetTotalTime();
		
	//クォータニオンを取得.
	D3DXQUATERNION quat = m_pPlayer.GetQuaternion();
	//正規化.
	D3DXQuaternionNormalize(&quat, &quat);

	//クォータニオンの設定.
	m_pPlayer.SetQuaternion(
		0.f, quat.y, 0.f, quat.w);
}

//--- 状態の終了時に呼び出す ---.
void CPlayerHoldingIdleState::Exit()
{
	//クォータニオンを取得.
	D3DXQUATERNION quat = m_pPlayer.GetQuaternion();
	//正規化.
	D3DXQuaternionNormalize(&quat, &quat);

	//クォータニオンの設定.
	m_pPlayer.SetQuaternion(quat);
}

//--- この状態の間に呼び出す ---.
void CPlayerHoldingIdleState::Update()
{
	ItemBase* item = m_pPlayer.GetItemBase();

	if (!item)
	{
		m_pPlayer.SetActionState(std::make_unique<CPlayerActionIdleState>(m_pPlayer));
		return;
	}

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();

	//ローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffsetPos = m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffsetPos = m_pPlayer.GetPlayerLeftHand().GetOffsetPos();

	if (dynamic_cast<Haetataki*>(item)
		|| dynamic_cast<SmashBat*>(item))
	{
		rightHandOffsetPos += m_OneHand_RightHandEndPos;
		leftHandOffsetPos += m_OneHand_LeftHandEndPos;
	}
	else if (dynamic_cast<Bomb*>(item)
		|| dynamic_cast<Mushroom*>(item)
		|| dynamic_cast<Fun*>(item)
		|| dynamic_cast<TrackingRobot*>(item))
	{
		rightHandOffsetPos += m_HoldBothHands_RightHandEndPos;
		leftHandOffsetPos += m_HoldBothHands_LeftHandEndPos;
	}

	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	//終了時間を過ぎた場合.
	if (t - m_StartTime > m_EndTime)
	{
		//アニメーション開始時間.
		m_StartTime = CTimeManager::GetTotalTime();
	}

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = progress = std::clamp(progress, 0.f, 1.f);

	float eased = cosf(progress * D3DX_PI * 2.f) * 0.08f;	//1周分の補正を計算.	

	//調整位置に足す.
	rightHandOffsetPos.y += eased;
	leftHandOffsetPos.y += eased;

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(rightHandOffsetPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(leftHandOffsetPos));
}
