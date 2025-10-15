#include "CPlayerHandAttackState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerHandWhiffState/CPlayerHandWhiffState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerHandHitState/CPlayerHandHitState.h"

CPlayerHandAttackState::CPlayerHandAttackState()
	: m_CenterHandOffset	( 0.2f )

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

void CPlayerHandAttackState::Enter(CPlayerBase& pPlayerBase)
{
	//SEを鳴らす.
	AssetManager::Sound()->PlaySE(enSoundList::SE_AttackHand);

	pPlayerBase.SetAttacking(true);

	//攻撃の開始時間を取得.
	m_StartTime = static_cast<float>(CTimeManager::GetTotalTime());

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayerBase.GetPosition();
	m_StartQuat = pPlayerBase.GetQuaternion();

	//ローカル軸を取得.
	CPlayerBase::LocalAxes axes = pPlayerBase.GetLocalAxes();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = pPlayerBase.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = pPlayerBase.GetPlayerLeftHand().GetOffsetPos();

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

void CPlayerHandAttackState::Exit(CPlayerBase& pPlayerBase)
{
}

void CPlayerHandAttackState::Update(CPlayerBase& pPlayerBase)
{
	//ゲーム全体の経過時間.
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	bool isHit = pPlayerBase.GetHitInfo().isHit;

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	//攻撃が当たってる場合.
	if (t - m_StartTime > m_EndTime
		&& isHit)
	{
		pPlayerBase.SetAttacking(false);
		pPlayerBase.SetActionState(std::make_unique<CPlayerHandHitState>());
		pPlayerBase.SetHitInfo(false, CPlayerBase::PlayerEvent::None);
		return;
	}
	//攻撃が当たっていない場合.
	else if (t - m_StartTime > m_EndTime)
	{
		pPlayerBase.SetActionState(std::make_unique<CPlayerHandWhiffState>());
		return;
	}

	//ローカル軸を取得.
	CPlayerBase::LocalAxes axes = pPlayerBase.GetLocalAxes();

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = pPlayerBase.Clamp(progress, 0.f, 1.f);

	//現在の傾き = 最大傾き角度 * 割合.
	m_CurrentTiltAngle = pPlayerBase.WrapAngle(m_TiltAngleMax * progress);

	//クォータニオンの回転を計算して設定する.
	pPlayerBase.SetQuaternion(pPlayerBase.TiltedQuat(m_StartQuat, axes.right, m_CurrentTiltAngle));

	float eased = sinf(progress * D3DX_PI * 0.5f);	//0.5で半往復させ前に手を出す計算をする.	

	//右手と左手の調整位置だけの計算.
	D3DXVECTOR3 rightHandOffsetPos;
	D3DXVec3Lerp(&rightHandOffsetPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	D3DXVECTOR3 leftHandOffsetPos;
	D3DXVec3Lerp(&leftHandOffsetPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//プレイヤーの位置と手の調整位置を合わせる.
	D3DXVECTOR3 rightHandPos = pPlayerBase.GetObjectPos(rightHandOffsetPos);
	D3DXVECTOR3 leftHandPos = pPlayerBase.GetObjectPos(leftHandOffsetPos);

	//手の位置を設定.
	pPlayerBase.GetPlayerRightHand().SetPosition(rightHandPos);
	pPlayerBase.GetPlayerLeftHand().SetPosition(leftHandPos);
}
