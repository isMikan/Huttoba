#include "CPlayerResultWin_TypeA.h"

#include "PlayerBase/CPlayerBase.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandWhiffState/CPlayerHandWhiffState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandHitState/CPlayerHandHitState.h"

CPlayerResultWin_TypeA::CPlayerResultWin_TypeA(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )
	
	, m_StartTime			()
	, m_EndTime				( 0.9f )

	, m_CurrentTiltAngle	()
	, m_TiltAngleMax		( D3DXToRadian( -10.f ) )
	, m_PhaseSplit			( 0.7f )
	, m_HandLaps			( 2.f )		//一周.
	, m_HandWidth			( 0.045f )

	, m_RightHandPos		()
	, m_LeftHandPos			()
	, m_RightHandStartPos	( -0.1f, -0.1f, 0.2f )
	, m_LeftHandStartPos	( 0.1f, -0.1f, 0.2f )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerResultWin_TypeA::~CPlayerResultWin_TypeA()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerResultWin_TypeA::Enter()
{
	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetTotalTime();

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();
	m_StartQuat = m_pPlayer.GetQuaternion();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = m_pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//手の開始位置を設定.
	m_RightHandStartPos += rightHandOffset;
	m_LeftHandStartPos += leftHandOffset;
	m_RightHandPos = m_RightHandStartPos;
	m_LeftHandPos = m_LeftHandStartPos;
}

//--- 状態の終了時に呼び出す ---.
void CPlayerResultWin_TypeA::Exit()
{
}

//--- この状態の間に呼び出す ---.
void CPlayerResultWin_TypeA::Update()
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

	//時間の割合が半分より前なら（傾く動き）.
	if (progress < m_PhaseSplit)
	{
		//後ろに傾くまでの現在の傾き割合.
		float ratio = progress / m_PhaseSplit;
		m_TiltAngleMax = D3DXToRadian(-15.f);	//10度前に

		//現在の傾き = 最大傾き角度 * 割合.
		m_CurrentTiltAngle = m_pPlayer.WrapAngle(m_TiltAngleMax * ratio);
	}
	//時間の割合が半分以上(戻る動き).
	else if (progress <= 1.0f)
	{
		//傾きの変わり目(m_PhaseSplit)からどれだけ経過したかを割って割合.
		float ratio = (progress - m_PhaseSplit) / m_PhaseSplit;
		m_TiltAngleMax = D3DXToRadian(-5.f);		//5度前に.
		
		//現在の傾き = 最大傾き角度 * (1 - 割合).
		m_CurrentTiltAngle = m_pPlayer.WrapAngle(m_TiltAngleMax * (1.f - ratio));
	}
	else
	{
		//終了後は0度.
		m_CurrentTiltAngle = 0.f;
	}

	//ローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();
	//クォータニオンの回転を計算して設定する.
	m_pPlayer.SetQuaternion(m_pPlayer.TiltedQuat(m_StartQuat, axes.right, m_CurrentTiltAngle));

	//上に手を出す計算をする.	
	float eased = sinf(progress * D3DX_PI * m_HandLaps) * m_HandWidth;	

	//調整位置に足す.
	m_RightHandPos.y += eased;
	m_LeftHandPos.y += eased;

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(m_RightHandPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(m_LeftHandPos));
}
