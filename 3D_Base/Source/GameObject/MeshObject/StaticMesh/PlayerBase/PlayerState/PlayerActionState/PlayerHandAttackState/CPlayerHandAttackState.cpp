#include "CPlayerHandAttackState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerHandWhiffState/CPlayerHandWhiffState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerHandHitState/CPlayerHandHitState.h"

CPlayerHandAttackState::CPlayerHandAttackState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )
	
	, m_CenterHandOffset	( 0.2f )

	, m_StartTime			()
	, m_EndTime				( 0.1f )

	, m_CurrentTiltAngle	()
	, m_TiltAngleMax		( D3DXToRadian( 10.f ) )

	, m_RightHandStartPos	()
	, m_LeftHandStartPos	()
	, m_RightHandEndPos		( 0.f, 0.f, 0.6f )
	, m_LeftHandEndPos		( 0.f, 0.f, 0.6f )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerHandAttackState::~CPlayerHandAttackState()
{
}

void CPlayerHandAttackState::Enter()
{
	//SEを鳴らす.
	AssetManager::Sound()->PlaySE(enSoundList::SE_AttackHand);

	m_pPlayer.SetPlayerEvent(CPlayerBase::PlayerEvent::HandAttack);

	//攻撃の開始時間を取得.
	m_StartTime = static_cast<float>(CTimeManager::GetTotalTime());

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
	//手の位置を中心寄りに調整.
	m_RightHandStartPos.x = rightHandOffset.x - m_CenterHandOffset;
	m_LeftHandStartPos.x = leftHandOffset.x + m_CenterHandOffset;
	//手の終了位置を設定.
	m_RightHandEndPos = m_RightHandStartPos + m_RightHandEndPos;
	m_LeftHandEndPos = m_LeftHandStartPos + m_LeftHandEndPos;
}

void CPlayerHandAttackState::Exit()
{
}

void CPlayerHandAttackState::Update()
{
	//ゲーム全体の経過時間.
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	bool isHit = m_pPlayer.GetHitInfo().isHit;

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	//攻撃が当たってる場合.
	if (t - m_StartTime > m_EndTime
		&& isHit)
	{
		m_pPlayer.SetHitInfo(false, CPlayerBase::HitEvent::None);
		m_pPlayer.SetActionState(std::make_unique<CPlayerHandHitState>(m_pPlayer));
		return;
	}
	//攻撃が当たっていない場合.
	else if (t - m_StartTime > m_EndTime)
	{
		m_pPlayer.SetActionState(std::make_unique<CPlayerHandWhiffState>(m_pPlayer));
		return;
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
	D3DXVec3Lerp(&rightHandOffsetPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	D3DXVECTOR3 leftHandOffsetPos;
	D3DXVec3Lerp(&leftHandOffsetPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(rightHandOffsetPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(leftHandOffsetPos));
}
