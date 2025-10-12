#include "CPlayerHandAttackState.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerActionState/PlayerHandWhiffState/CPlayerHandWhiffState.h"
#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerActionState/PlayerHandHitState/CPlayerHandHitState.h"

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

void CPlayerHandAttackState::Enter(CPlayer& pPlayer)
{
	//SEを鳴らす.
	AssetManager::Sound()->PlaySE(enSoundList::SE_AttackHand);

	pPlayer.SetAttacking(true);

	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetTotalTime();

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();
	m_StartQuat = pPlayer.GetQuaternion();

	//ローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = pPlayer.GetPlayerLeftHand().GetOffsetPos();

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

void CPlayerHandAttackState::Exit(CPlayer& pPlayer)
{
}

void CPlayerHandAttackState::Update(CPlayer& pPlayer)
{
	//ゲーム全体の経過時間.
	float t = CTimeManager::GetTotalTime();

	bool isHit = pPlayer.GetHitInfo().isHit;

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	//攻撃が当たってる場合.
	if (t - m_StartTime > m_EndTime
		&& isHit)
	{
		pPlayer.SetAttacking(false);
		pPlayer.SetActionState(std::make_unique<CPlayerHandHitState>());
		pPlayer.SetHitInfo(false, CPlayer::StatePattern::None);
		return;
	}
	//攻撃が当たっていない場合.
	else if (t - m_StartTime > m_EndTime)
	{
		pPlayer.SetActionState(std::make_unique<CPlayerHandWhiffState>());
		return;
	}

	//ローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = pPlayer.Clamp(progress, 0.f, 1.f);

	//現在の傾き = 最大傾き角度 * 割合.
	m_CurrentTiltAngle = pPlayer.WrapAngle(m_TiltAngleMax * progress);

	//クォータニオンの回転を計算して設定する.
	pPlayer.SetQuaternion(pPlayer.TiltedQuat(m_StartQuat, axes.right, m_CurrentTiltAngle));

	float eased = sinf(progress * D3DX_PI * 0.5f);	//0.5で半往復させ前に手を出す計算をする.	

	//右手と左手の調整位置だけの計算.
	D3DXVECTOR3 rightHandOffsetPos;
	D3DXVec3Lerp(&rightHandOffsetPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	D3DXVECTOR3 leftHandOffsetPos;
	D3DXVec3Lerp(&leftHandOffsetPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//攻撃の開始時間を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();

	//方向に合わせて右手の位置を調整.
	rightHandOffsetPos =
		axes.right * rightHandOffsetPos.x +
		axes.up * rightHandOffsetPos.y +
		axes.forward * rightHandOffsetPos.z;

	//方向に合わせて左手の位置を調整.
	leftHandOffsetPos =
		axes.right * leftHandOffsetPos.x +
		axes.up * leftHandOffsetPos.y +
		axes.forward * leftHandOffsetPos.z;

	//プレイヤーの位置と手の調整位置を合わせる.
	D3DXVECTOR3 rightHandPos = playerPos + rightHandOffsetPos;
	D3DXVECTOR3 leftHandPos = playerPos + leftHandOffsetPos;

	//手の位置を設定.
	pPlayer.GetPlayerRightHand().SetPosition(rightHandPos);
	pPlayer.GetPlayerLeftHand().SetPosition(leftHandPos);
}
