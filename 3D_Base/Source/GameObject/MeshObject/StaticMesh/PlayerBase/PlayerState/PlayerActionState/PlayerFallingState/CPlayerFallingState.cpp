#include "CPlayerFallingState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"
										   
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerGetUpState/CPlayerGetUpState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"

CPlayerFallingState::CPlayerFallingState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )

	, m_UpHandOffset		( 0.f, 0.5f, 0.f )
	, m_StartRightAxis		()
	, m_Velocity			()

	, m_StartTime			()
	, m_EndTime				( 10.f )	//吹き飛ばし量によって着地時間が変わるので多めに.

	, m_GroundRange			( 1.5f )					//この位置を下回るまで回転. 
	, m_RotateRange			( D3DXToRadian( 15.f ) )	//この角度の範囲内で止まる.
	, m_ForceMax			( 15.f )					//想定.

	, m_Gravity				( -9.8f )
	, m_RotateSpeed			( 40.f )	//20回転.
	, m_CurrentTiltAngle	()

	, m_GroundPos			( 0.1f )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerFallingState::~CPlayerFallingState()
{
}

void CPlayerFallingState::Enter()
{
	m_pPlayer.SetPlayerEvent(CPlayerBase::PlayerEvent::Falling);

	//攻撃の開始時間を取得.
	m_StartTime = static_cast<float>(CTimeManager::GetTotalTime());

	//傾き角度の初期化.
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
	m_StartQuat = m_pPlayer.GetQuaternion();

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();
	//開始時の右軸を設定.
	m_StartRightAxis = -axes.right;

	//初速度を設定.
	m_Velocity = m_pPlayer.GetHitInfo().velocity;
	m_Velocity.y = 0.f;

	//吹き飛ばし量を取得.
	float force = m_pPlayer.GetHitInfo().force;
	m_RotateSpeed += m_ForceMax - force;	//最大量から引いて速さの調整.
}

void CPlayerFallingState::Exit()
{
	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();
	//地面に着地.
	m_pPlayer.SetPosition(playerPos.x, m_GroundPos, playerPos.z);
}

void CPlayerFallingState::Update()
{
	//経過時間を取得.
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime
		|| IsEnd())
	{
		if (m_pPlayer.GetHitInfo().hitEvent == CPlayerBase::HitEvent::Knockback)
		{
			m_pPlayer.SetActionState(std::make_unique<CPlayerGetUpState>(m_pPlayer));
		}
		else if(m_pPlayer.GetHitInfo().hitEvent == CPlayerBase::HitEvent::Knockdown)
		{
			m_pPlayer.SetActionState(std::make_unique<CPlayerKnockdownState>(m_pPlayer));
		}
		return;
	}

	//現在のクォータニオンを取得.
	D3DXQUATERNION quat = m_pPlayer.GetQuaternion();

	m_CurrentTiltAngle = m_pPlayer.WrapAngle(m_CurrentTiltAngle);
	//地面近くかつ90度に近づいたら.
	if (playerPos.y < m_GroundRange
		&& fabsf(WorldAngle() + D3DXToRadian(90.f)) < m_RotateRange)
	{
		//正規化.
		D3DXQuaternionNormalize(&quat, &quat);
		//現在のクォータニオンを設定.
		m_pPlayer.SetQuaternion(quat);
	}
	else
	{
		//全体の時間の現在の割合.
		float progress = (t - m_StartTime) / m_EndTime;
		progress = std::clamp(progress, 0.f, 1.f);

		//時間以内に回数分回転するように.
		m_CurrentTiltAngle = m_pPlayer.WrapAngle(progress * D3DX_PI * m_RotateSpeed);

		//クォータニオンの回転を計算して設定.
		m_pPlayer.SetQuaternion(m_pPlayer.TiltedQuat(m_StartQuat, m_StartRightAxis, m_CurrentTiltAngle));
	}

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	//手の調整位置の設定.
	D3DXVECTOR3 rightHandOffsetPos = m_UpHandOffset + m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffsetPos = m_UpHandOffset + m_pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//プレイヤーの位置と手の調整位置を合わせる.
	D3DXVECTOR3 rightHandPos = m_pPlayer.GetObjectPos(rightHandOffsetPos);
	D3DXVECTOR3 leftHandPos = m_pPlayer.GetObjectPos(leftHandOffsetPos);

	//手の位置を設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(rightHandPos);
	m_pPlayer.GetPlayerLeftHand().SetPosition(leftHandPos);

	//1フレームの速さを取得.
	float dt = static_cast<float>(CTimeManager::GetDeltaTime());

	//地面についた場合.
	if (playerPos.y <= m_GroundPos)
	{
		playerPos.y = m_GroundPos;	//位置をそろえておく.
	}
	else
	{
		//飛んでいく移動量の計算.
		m_Velocity.y += m_Gravity * dt;
		//プレイヤーに移動量を足す.
		playerPos += m_Velocity * dt;
	}
	//プレイヤーの位置を設定.
	m_pPlayer.SetPosition(playerPos);
}

float CPlayerFallingState::WorldAngle()
{
	//上方向のローカル軸を取得.
	D3DXVECTOR3 localUp = m_pPlayer.GetLocalAxes().up;
	//上方向.
	D3DXVECTOR3 up(0.f, 1.f, 0.f);

	float dot = D3DXVec3Dot(&localUp, &up);
	dot = std::clamp(dot, -1.f, 1.f);

	float angle = acosf(dot);

	return -angle;
}

bool CPlayerFallingState::IsEnd()
{
	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();

	//回転を90度付近で止め、位置が地面についたら.
	if (fabsf(WorldAngle() + D3DXToRadian(90.f)) < m_RotateRange
		&& playerPos.y <= m_GroundPos)
	{
		return true;
	}

	return false;
}
