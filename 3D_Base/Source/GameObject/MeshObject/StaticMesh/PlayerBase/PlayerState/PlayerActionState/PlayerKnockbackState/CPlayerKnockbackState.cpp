#include "CPlayerKnockbackState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"
									   
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerFallingState/CPlayerFallingState.h"

CPlayerKnockbackState::CPlayerKnockbackState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )
	
	, m_UpHandOffset		( 0.f, 0.5f, 0.f )
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

void CPlayerKnockbackState::Enter()
{
	//SEを鳴らす.
	AssetManager::Sound()->PlaySE(enSoundList::SE_Knockback);
	
	m_pPlayer.SetPlayerEvent(CPlayerBase::PlayerEvent::Knockback);

	//攻撃の開始時間を取得.
	m_StartTime = static_cast<float>(CTimeManager::GetTotalTime());

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();
	//攻撃された情報の取得.
	CPlayerBase::HitInfo hitInfo = m_pPlayer.GetHitInfo();
	
	//傾き角度の初期化.
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
	m_StartQuat = m_pPlayer.GetQuaternion();

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();
	//開始時の右軸を設定.
	m_StartRightAxis = -axes.right;

	//初速度を設定.
	m_Velocity = m_pPlayer.GetVelocity();

	//位置を設定.
	m_pPlayer.SetHitInfo(
		hitInfo.position, m_Velocity, hitInfo.force,
		false, hitInfo.hitEvent);
}

void CPlayerKnockbackState::Exit()
{
}

void CPlayerKnockbackState::Update()
{
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime)
	{
		m_pPlayer.SetActionState(std::make_unique<CPlayerFallingState>(m_pPlayer));
		return;
	}

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	//手の調整位置の設定.
	D3DXVECTOR3 rightHandOffsetPos = m_UpHandOffset + m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffsetPos = m_UpHandOffset + m_pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(rightHandOffsetPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(leftHandOffsetPos));

	float dt = static_cast<float>(CTimeManager::GetDeltaTime());

	//飛んでいく移動量の計算.
	m_Velocity.y += m_Gravity * dt;
	//プレイヤーに移動量を足す.
	playerPos += m_Velocity * dt;
	//プレイヤーの位置を設定.
	m_pPlayer.SetPosition(playerPos);

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = std::clamp(progress, 0.f, 1.f);
	
	//時間以内に回数分回転するように.
	m_CurrentTiltAngle = m_pPlayer.WrapAngle(progress * D3DX_PI * m_RotateSpeed);

	//クォータニオンの回転を計算して設定する.
	m_pPlayer.SetQuaternion(
		m_pPlayer.TiltedQuat(m_StartQuat, m_StartRightAxis, m_CurrentTiltAngle));
}
