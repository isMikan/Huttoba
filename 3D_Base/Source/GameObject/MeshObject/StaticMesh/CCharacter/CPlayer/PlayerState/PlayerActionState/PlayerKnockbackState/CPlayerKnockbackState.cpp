#include "CPlayerKnockback.h"

#include "CPlayer.h"

#include "CPlayerMoveIdle.h"
#include "CPlayerRotationIdle.h"
#include "CPlayerActionIdle.h"

#include "CPlayerFallDown.h"

#include "CGameTimer.h"

CPlayerKnockback::CPlayerKnockback()
	: m_UpHandOffset		( 0.f, 0.5f, 0.f )
	, m_StartRightAxis		()
	, m_Velocity			()

	, m_StartTime			()
	, m_EndTime				( 1.f )

	, m_Gravity				( 9.8f )
	, m_RotateSpeed			( 4.f )
	, m_CurrentTiltAngle	()
	, m_TiltAngleMax		( D3DXToRadian( -30.f ) )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerKnockback::~CPlayerKnockback()
{
}

void CPlayerKnockback::Enter(CPlayer& pPlayer)
{
	//攻撃の開始時間を取得.
	m_StartTime = CGameTimer::GetInstance()->GetTotalTime();

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();
	//攻撃された情報の取得.
	CPlayer::HitInfo hitInfo = pPlayer.GetHitInfo();
	
	//傾き角度の初期化.
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
	m_StartQuat = pPlayer.GetRotationQuat();

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

	D3DXQUATERNION playerQuat;

	D3DXVECTOR3 vec = hitInfo.position - playerPos;

	D3DXVec3Normalize(&vec, &vec);

	float angle = atan2f(vec.z, vec.x);
	D3DXVECTOR3 up(0.f, 1.f, 0.f);
	D3DXQUATERNION quat;
	D3DXQuaternionRotationAxis(&quat, &up, angle);
	D3DXQuaternionNormalize(&quat, &quat);
	D3DXQuaternionMultiply(&playerQuat, &playerQuat, &quat);

	//クォータニオンの回転を設定.
	pPlayer.SetRotationQuat(playerQuat);
}

void CPlayerKnockback::Exit(CPlayer& pPlayer)
{
}

void CPlayerKnockback::Update(CPlayer& pPlayer)
{
	pPlayer.SetMoveState(std::make_unique<CPlayerMoveIdle>());
	pPlayer.SetRotationState(std::make_unique<CPlayerRotationIdle>());

	float t = CGameTimer::GetInstance()->GetTotalTime();

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime)
	{
		pPlayer.SetActionState(std::make_unique<CPlayerFallDown>());
		return;
	}

	//プレイヤーのローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

	//手の調整位置の設定.
	D3DXVECTOR3 rightHandOffsetPos = m_UpHandOffset + pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffsetPos = m_UpHandOffset + pPlayer.GetPlayerLeftHand().GetOffsetPos();

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

	float dt = CGameTimer::GetInstance()->GetDeltaTime();

	//攻撃された情報の取得.
	m_Velocity.y += -m_Gravity * dt * 0.1f;
	playerPos += m_Velocity * dt;

	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &pPlayer.GetRotationQuat());
	//プレイヤーの位置を設定.
	pPlayer.SetPosition(playerPos);

	if (playerPos.y < 5.f
		&& D3DXToRadian(pPlayer.GetRotationQuat().y) < D3DXToRadian(100.f)
		&& D3DXToRadian(pPlayer.GetRotationQuat().y) > D3DXToRadian(80.f))
	{
		D3DXQUATERNION base(0.f, m_StartQuat.y, 0.f, m_StartQuat.w);
		pPlayer.SetRotationQuat(
			pPlayer.TiltedQuat(base, m_StartRightAxis, D3DXToRadian(90.f)));
		return;
	}

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;

	m_CurrentTiltAngle = progress * D3DX_PI * m_RotateSpeed;

	//クォータニオンの回転を計算して設定する.
	pPlayer.SetRotationQuat(
		pPlayer.TiltedQuat(m_StartQuat, m_StartRightAxis, m_CurrentTiltAngle));
}

void CPlayerKnockback::Handle(CPlayer& pPlayer, int inputKey)
{
}

std::string CPlayerKnockback::GetStateName() const
{
	return std::string();
}
