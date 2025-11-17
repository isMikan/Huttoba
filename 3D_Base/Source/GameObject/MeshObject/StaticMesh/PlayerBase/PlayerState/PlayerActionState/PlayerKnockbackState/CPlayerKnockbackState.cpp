#include "CPlayerKnockbackState.h"

#include "PlayerBase/CPlayerBase.h"
			
#include "PlayerBase/PlayerState/PlayerActionState/PlayerFallingState/CPlayerFallingState.h"

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

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerKnockbackState::Enter()
{
	//SEを鳴らす.
	AssetManager::Sound()->PlaySE(enSoundList::SE_Knockback);
	
	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetTotalTime();

	//攻撃された情報の取得.
	CPlayerBase::HitAttack hitInfo = m_pPlayer.GetHitAttack();
	
	//傾き角度の初期化.
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
	m_StartQuat = m_pPlayer.GetQuaternion();

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();
	//開始時の右軸を設定.
	m_StartRightAxis = -axes.right;

	//初速度を設定.
	m_Velocity = m_pPlayer.GetHitAttack().velocity;

	//吹き飛ばす攻撃を受けた場合.
	if (m_pPlayer.GetHitAttack().hitEvent == CPlayerBase::HitEvent::Knockback)
	{
		m_pPlayer.SetHitAttack(m_Velocity, CPlayerBase::HitEvent::NoDown);
	}
	//ダウン状態付きの攻撃を受けた場合.
	else if (m_pPlayer.GetHitAttack().hitEvent == CPlayerBase::HitEvent::Knockdown)
	{
		m_pPlayer.SetHitAttack(m_Velocity, CPlayerBase::HitEvent::WithDown);
	}
}

//--- 状態の終了時に呼び出す ---.
void CPlayerKnockbackState::Exit()
{
}

//--- この状態の間に呼び出す ---.
void CPlayerKnockbackState::Update()
{
	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

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

	float dt = CTimeManager::GetDeltaTime();

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
