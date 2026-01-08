#include "CPlayerResultWin_TypeB.h"

#include "PlayerBase/CPlayerBase.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandWhiffState/CPlayerHandWhiffState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandHitState/CPlayerHandHitState.h"

CPlayerResultWin_TypeB::CPlayerResultWin_TypeB(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )
	
	, m_StartTime			()
	, m_EndTime				( 0.3f )

	, m_HandLaps			( 1.f )		//一周.
	, m_HandWidth			( 0.4f )

	, m_RightHandPos		()
	, m_LeftHandPos			()
	, m_RightHandStartPos	( -0.4f, 0.2f, 0.4f )
	, m_LeftHandStartPos	(  0.4f, 0.2f, 0.4f )
	, m_RightHandEndPos		(  0.4f, 0.2f, 0.4f )
	, m_LeftHandEndPos		( -0.4f, 0.2f, 0.4f )
{
}

CPlayerResultWin_TypeB::~CPlayerResultWin_TypeB()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerResultWin_TypeB::Enter()
{
	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetTotalTime();

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = m_pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//手の開始位置を設定.
	m_RightHandStartPos += rightHandOffset;
	m_LeftHandStartPos += leftHandOffset;
	m_RightHandPos = m_RightHandStartPos;
	m_LeftHandPos = m_LeftHandStartPos;
	m_RightHandEndPos += rightHandOffset;
	m_LeftHandEndPos += leftHandOffset;
}

//--- 状態の終了時に呼び出す ---.
void CPlayerResultWin_TypeB::Exit()
{
}

//--- この状態の間に呼び出す ---.
void CPlayerResultWin_TypeB::Update()
{
	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	//終了時間を過ぎた場合.
	if (t - m_StartTime > m_EndTime)
	{
		//念のためずれないように手の開始位置を設定.
		m_RightHandPos = m_RightHandStartPos;
		m_LeftHandPos = m_LeftHandStartPos;
		//アニメーション開始時間.
		m_StartTime = CTimeManager::GetTotalTime();
	}

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = progress = std::clamp(progress, 0.f, 1.f);

	//横に手を移動する計算.	
	float eased = sinf(progress * D3DX_PI * m_HandLaps) * m_HandWidth;

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
