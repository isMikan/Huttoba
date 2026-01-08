#include "CPlayerResultWin_TypeD.h"

#include "PlayerBase/CPlayerBase.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandWhiffState/CPlayerHandWhiffState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandHitState/CPlayerHandHitState.h"

CPlayerResultWin_TypeD::CPlayerResultWin_TypeD(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )
	
	, m_StartTime			()
	, m_EndTime				( 2.f )

	, m_HandLaps			( 2.f )		//一周.
	, m_HandWidth			( 0.001f )

	, m_RightHandPos		()
	, m_LeftHandPos			()
	, m_RightHandEndPos		( 0.2f, 0.3f, 0.4f )
	, m_LeftHandEndPos		( 0.f, 0.f, -0.2f )
{
}

CPlayerResultWin_TypeD::~CPlayerResultWin_TypeD()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerResultWin_TypeD::Enter()
{
	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetTotalTime();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = m_pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//手の開始位置を設定.
	m_RightHandPos = m_RightHandEndPos + rightHandOffset;
	m_LeftHandPos = m_LeftHandEndPos + leftHandOffset;

	m_pPlayer.SetQuaternion(0.f, D3DXToRadian(-90.f), D3DXToRadian(5.f), 1.f);
}

//--- 状態の終了時に呼び出す ---.
void CPlayerResultWin_TypeD::Exit()
{
}

//--- この状態の間に呼び出す ---.
void CPlayerResultWin_TypeD::Update()
{
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

	float eased = cosf(progress * D3DX_PI * m_HandLaps) * m_HandWidth;	//1周分の補正を計算.	

	//調整位置に足す.
	m_RightHandPos.y += eased;
	m_LeftHandPos.y += eased;

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(m_RightHandPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(m_LeftHandPos));
}
