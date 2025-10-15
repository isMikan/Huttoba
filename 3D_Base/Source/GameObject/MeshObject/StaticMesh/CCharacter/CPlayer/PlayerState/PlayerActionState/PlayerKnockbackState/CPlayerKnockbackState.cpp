#include "CPlayerKnockbackState.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerActionState/PlayerFallingState/CPlayerFallingState.h"

CPlayerKnockbackState::CPlayerKnockbackState()
	: m_UpHandOffset		( 0.f, 0.5f, 0.f )
	, m_StartRightAxis		()
	, m_Velocity			()

	, m_StartTime			()
	, m_EndTime				( 1.f )

	, m_Gravity				( -9.8f )
	, m_RotateSpeed			( 4.f )		//2回転.
	, m_CurrentTiltAngle	()

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerKnockbackState::~CPlayerKnockbackState()
{
}

void CPlayerKnockbackState::Enter(CPlayer& pPlayer)
{
	//攻撃の開始時間を取得.
	m_StartTime = static_cast<float>(CTimeManager::GetTotalTime());

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();
	//攻撃された情報の取得.
	CPlayer::HitInfo hitInfo = pPlayer.GetHitInfo();
	
	//傾き角度の初期化.
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
	m_StartQuat = pPlayer.GetQuaternion();

	//プレイヤーのローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();
	//開始時の右軸を設定.
	m_StartRightAxis = -axes.right;

	//初速度を設定.
	m_Velocity = pPlayer.GetVelocity();

	//位置を設定.
	pPlayer.SetHitInfo(
		hitInfo.position, m_Velocity, hitInfo.force,
		false, hitInfo.animName);
}

void CPlayerKnockbackState::Exit(CPlayer& pPlayer)
{
}

void CPlayerKnockbackState::Update(CPlayer& pPlayer)
{
	pPlayer.SetMoveState(std::make_unique<CPlayerMoveIdleState>());
	pPlayer.SetTurnState(std::make_unique<CPlayerTurnIdleState>());

	float t = static_cast<float>(CTimeManager::GetTotalTime());

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime)
	{
		pPlayer.SetActionState(std::make_unique<CPlayerFallingState>());
		return;
	}

	//プレイヤーのローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

	//手の調整位置の設定.
	D3DXVECTOR3 rightHandOffsetPos = m_UpHandOffset + pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffsetPos = m_UpHandOffset + pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//プレイヤーの位置と手の調整位置を合わせる.
	D3DXVECTOR3 rightHandPos = pPlayer.GetObjectPos(rightHandOffsetPos);
	D3DXVECTOR3 leftHandPos = pPlayer.GetObjectPos(leftHandOffsetPos);

	//手の位置を設定.
	pPlayer.GetPlayerRightHand().SetPosition(rightHandPos);
	pPlayer.GetPlayerLeftHand().SetPosition(leftHandPos);

	float dt = static_cast<float>(CTimeManager::GetDeltaTime());

	//攻撃された情報の取得.
	m_Velocity.y += m_Gravity * dt;
	playerPos += m_Velocity * dt;

	//プレイヤーの位置を設定.
	pPlayer.SetPosition(playerPos);

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	//時間以内に回数分回転するように.
	m_CurrentTiltAngle = pPlayer.WrapAngle(progress * D3DX_PI * m_RotateSpeed);

	//クォータニオンの回転を計算して設定する.
	pPlayer.SetQuaternion(
		pPlayer.TiltedQuat(m_StartQuat, m_StartRightAxis, m_CurrentTiltAngle));
}
