#include "CPlayerFallingState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
										   
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerGetUpState/CPlayerGetUpState.h"

CPlayerFallingState::CPlayerFallingState()
	: m_UpHandOffset		( 0.f, 0.5f, 0.f )
	, m_StartRightAxis		()
	, m_Velocity			()

	, m_StartTime			()
	, m_EndTime				( 10.f )	//吹き飛ばし量によって着地時間が変わるので多めに.

	, m_GroundRange			( 1.5f )	//この位置を下回るまで回転. 
	, m_RotateRange			( 15.f )	//この角度の範囲内で止まる.
	, m_ForceMax			( 14.f )	//想定.

	, m_Gravity				( -9.8f )
	, m_RotateSpeed			( 40.f )	//20回転.
	, m_CurrentTiltAngle	()

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerFallingState::~CPlayerFallingState()
{
}

void CPlayerFallingState::Enter(CPlayerBase& pPlayerBase)
{
	//攻撃の開始時間を取得.
	m_StartTime = static_cast<float>(CTimeManager::GetTotalTime());

	//傾き角度の初期化.
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
	m_StartQuat = pPlayerBase.GetQuaternion();

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = pPlayerBase.GetLocalAxes();
	//開始時の右軸を設定.
	m_StartRightAxis = -axes.right;

	//初速度を設定.
	m_Velocity = pPlayerBase.GetHitInfo().velocity;
	m_Velocity.y = 0.f;

	//吹き飛ばし量を取得.
	float force = pPlayerBase.GetHitInfo().force;
	m_RotateSpeed += m_ForceMax - force;	//最大量から引いて速さの調整.
}

void CPlayerFallingState::Exit(CPlayerBase& pPlayerBase)
{
	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayerBase.GetPosition();
	//床に着地.
	pPlayerBase.SetPosition(playerPos.x, 0.f, playerPos.z);
}

void CPlayerFallingState::Update(CPlayerBase& pPlayerBase)
{
	pPlayerBase.SetMoveState(std::make_unique<CPlayerMoveIdleState>());
	pPlayerBase.SetTurnState(std::make_unique<CPlayerTurnIdleState>());

	//経過時間を取得.
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayerBase.GetPosition();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime
		|| playerPos.y < 0.f)
	{
		pPlayerBase.SetActionState(std::make_unique<CPlayerGetUpState>());
		return;
	}

	//現在のクォータニオンを取得.
	D3DXQUATERNION quat = pPlayerBase.GetQuaternion();

	m_CurrentTiltAngle = pPlayerBase.WrapAngle(m_CurrentTiltAngle);
	//地面近くかつ90度に近づいたら.
	if (playerPos.y < m_GroundRange
		&& fabsf(D3DXToDegree(m_CurrentTiltAngle) - 90.f) < m_RotateRange)
	{
		//現在のクォータニオンを設定.
		pPlayerBase.SetQuaternion(quat);
	}
	else
	{
		//全体の時間の現在の割合.
		float progress = (t - m_StartTime) / m_EndTime;
		//時間以内に回数分回転するように.
		m_CurrentTiltAngle = pPlayerBase.WrapAngle(progress * D3DX_PI * m_RotateSpeed);

		//クォータニオンの回転を計算して設定.
		pPlayerBase.SetQuaternion(pPlayerBase.TiltedQuat(m_StartQuat, m_StartRightAxis, m_CurrentTiltAngle));
	}

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = pPlayerBase.GetLocalAxes();

	//手の調整位置の設定.
	D3DXVECTOR3 rightHandOffsetPos = m_UpHandOffset + pPlayerBase.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffsetPos = m_UpHandOffset + pPlayerBase.GetPlayerLeftHand().GetOffsetPos();

	//プレイヤーの位置と手の調整位置を合わせる.
	D3DXVECTOR3 rightHandPos = pPlayerBase.GetObjectPos(rightHandOffsetPos);
	D3DXVECTOR3 leftHandPos = pPlayerBase.GetObjectPos(leftHandOffsetPos);

	//手の位置を設定.
	pPlayerBase.GetPlayerRightHand().SetPosition(rightHandPos);
	pPlayerBase.GetPlayerLeftHand().SetPosition(leftHandPos);

	//1フレームの速さを取得.
	float dt = static_cast<float>(CTimeManager::GetDeltaTime());

	//攻撃された情報の取得.
	m_Velocity.y += m_Gravity * dt;
	playerPos += m_Velocity * dt;

	//プレイヤーの位置を設定.
	pPlayerBase.SetPosition(playerPos);
}