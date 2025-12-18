#include "CPlayerHandAttackState.h"

#include "PlayerBase/CPlayerBase.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandWhiffState/CPlayerHandWhiffState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHandHitState/CPlayerHandHitState.h"

CPlayerHandAttackState::CPlayerHandAttackState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )
	
	, m_CenterHandOffset	( 0.25f )

	, m_StartTime			()
	, m_EndTime				( 0.1f )

	, m_CurrentTiltAngle	()
	, m_TiltAngleMax		( D3DXToRadian( 10.f ) )
	, m_HandLaps			( 0.5f )	//半周.

	, m_RightHandStartPos	()
	, m_LeftHandStartPos	()
	, m_RightHandEndPos		( 0.f, 0.f, 0.75f )
	, m_LeftHandEndPos		( 0.f, 0.f, 0.75f )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerHandAttackState::~CPlayerHandAttackState()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerHandAttackState::Enter()
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
	m_RightHandStartPos = rightHandOffset;
	m_LeftHandStartPos = leftHandOffset;
	//手の位置を中心寄りに調整.
	m_RightHandStartPos.x = rightHandOffset.x - m_CenterHandOffset;
	m_LeftHandStartPos.x = leftHandOffset.x + m_CenterHandOffset;
	//手の終了位置を設定.
	m_RightHandEndPos = m_RightHandStartPos + m_RightHandEndPos;
	m_LeftHandEndPos = m_LeftHandStartPos + m_LeftHandEndPos;
}

//--- 状態の終了時に呼び出す ---.
void CPlayerHandAttackState::Exit()
{
}

//--- この状態の間に呼び出す ---.
void CPlayerHandAttackState::Update()
{
	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	//現在の経過時間と開始時間の差が終了時間を上回った場合.
	//攻撃が当たってる場合.
	if (t - m_StartTime > m_EndTime
		&& m_pPlayer.GetHitAttack().hitEvent == CPlayerBase::HitEvent::HandAttack)
	{
		m_pPlayer.SetHitAnim(CPlayerBase::HitEvent::None);
		m_pPlayer.SetActionState(std::make_unique<CPlayerHandHitState>(m_pPlayer));
		return;
	}
	//攻撃が当たっていない場合.
	else if (t - m_StartTime > m_EndTime)
	{
		m_pPlayer.SetActionState(std::make_unique<CPlayerHandWhiffState>(m_pPlayer));
		return;
	}

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = std::clamp(progress, 0.f, 1.f);

	//現在の傾き = 最大傾き角度 * 割合.
	m_CurrentTiltAngle = m_pPlayer.WrapAngle(m_TiltAngleMax * progress);

	//ローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();
	//クォータニオンの回転を計算して設定する.
	m_pPlayer.SetQuaternion(m_pPlayer.TiltedQuat(m_StartQuat, axes.right, m_CurrentTiltAngle));

	float eased = sinf(progress * D3DX_PI * m_HandLaps);	//前に手を出す計算をする.	

	//右手と左手の調整位置だけの計算.
	D3DXVECTOR3 rightHandOffsetPos;
	D3DXVec3Lerp(&rightHandOffsetPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	D3DXVECTOR3 leftHandOffsetPos;
	D3DXVec3Lerp(&leftHandOffsetPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(rightHandOffsetPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(leftHandOffsetPos));
}
