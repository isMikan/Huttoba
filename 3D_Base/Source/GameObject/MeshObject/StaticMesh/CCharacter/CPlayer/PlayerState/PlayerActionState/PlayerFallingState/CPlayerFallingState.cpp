#include "CPlayerFallingState.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerActionState/PlayerGetUpState/CPlayerGetUpState.h"

#include "TimeManager/CTimeManager.h"

CPlayerFallingState::CPlayerFallingState()
	: m_UpHandOffset		( 0.f, 0.5f, 0.f )
	, m_StartRightAxis		()
	, m_Velocity			()

	, m_StartTime			()
	, m_EndTime				( 10.f )	//吹き飛ばし量によって着地時間が変わるので多めに.

	, m_GroundRange			( 1.5f )	//この位置を下回るまで回転. 
	, m_RotateRange			( 15.f )	//この角度の範囲内で止まる.
	, m_ForceMax			( 14.f )	//想定.

	, m_Gravity				( 9.8f )
	, m_RotateSpeed			( 40.f )	//20回転.
	, m_CurrentTiltAngle	()

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerFallingState::~CPlayerFallingState()
{
}

void CPlayerFallingState::Enter(CPlayer& pPlayer)
{
	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetInstance()->GetTotalTime();

	//傾き角度の初期化.
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
	m_StartQuat = pPlayer.GetQuaternion();

	//プレイヤーのローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();
	//開始時の右軸を設定.
	m_StartRightAxis = -axes.right;

	//初速度を設定.
	m_Velocity = pPlayer.GetHitInfo().velocity;
	m_Velocity.y = 0.f;

	//吹き飛ばし量を取得.
	float force = pPlayer.GetHitInfo().force;
	m_RotateSpeed += m_ForceMax - force;	//最大量から引いて速さの調整.
}

void CPlayerFallingState::Exit(CPlayer& pPlayer)
{
	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();
	//床に着地.
	pPlayer.SetPosition(playerPos.x, 0.f, playerPos.z);
}

void CPlayerFallingState::Update(CPlayer& pPlayer)
{
	pPlayer.SetMoveState(std::make_unique<CPlayerMoveIdleState>());
	pPlayer.SetTurnState(std::make_unique<CPlayerTurnIdleState>());

	//経過時間を取得.
	float t = CTimeManager::GetInstance()->GetTotalTime();

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime
		|| playerPos.y < 0.f)
	{
		pPlayer.SetActionState(std::make_unique<CPlayerGetUpState>());
		return;
	}

	//現在のクォータニオンを取得.
	D3DXQUATERNION quat = pPlayer.GetQuaternion();

	m_CurrentTiltAngle = pPlayer.WrapAngle(m_CurrentTiltAngle);
	//地面近くかつ90度に近づいたら.
	if (playerPos.y < m_GroundRange
		&& fabsf(D3DXToDegree(m_CurrentTiltAngle) - 90.f) < m_RotateRange)
	{
		//現在のクォータニオンを設定.
		pPlayer.SetQuaternion(quat);
	}
	else
	{
		//全体の時間の現在の割合.
		float progress = (t - m_StartTime) / m_EndTime;
		//時間以内に回数分回転するように.
		m_CurrentTiltAngle = pPlayer.WrapAngle(progress * D3DX_PI * m_RotateSpeed);

		//クォータニオンの回転を計算して設定.
		pPlayer.SetQuaternion(pPlayer.TiltedQuat(m_StartQuat, m_StartRightAxis, m_CurrentTiltAngle));
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

	//1フレームの速さを取得.
	float dt = CTimeManager::GetInstance()->GetDeltaTime();

	//攻撃された情報の取得.
	m_Velocity.y += -m_Gravity * dt;
	playerPos += m_Velocity * dt;

	//プレイヤーの位置を設定.
	pPlayer.SetPosition(playerPos);
}