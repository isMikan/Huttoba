#include "CPlayerFallingState.h"

#include "PlayerBase/CPlayerBase.h"
			
#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHoldingIdleState/CPlayerHoldingIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerGetUpState/CPlayerGetUpState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"

CPlayerFallingState::CPlayerFallingState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )

	, m_UpHandOffset		( 0.f, 0.5f, 0.f )
	, m_StartRightAxis		()
	, m_Velocity			()

	, m_StartTime			()
	, m_EndTime				( 10.f )	//吹き飛ばし量によって着地時間が変わるので多めに.

	, m_GroundRange			( 1.5f )					//この位置を下回るまで回転. 
	, m_RotateRangeMax		( D3DXToRadian( 95.f ) )	//この角度の範囲内で止まる.
	, m_RotateRangeMin		( D3DXToRadian( 80.f ) )	//この角度の範囲内で止まる.
	, m_ForceMax			( 15.f )					//想定.

	, m_Gravity				( -9.8f )
	, m_RotateSpeed			( 40.f )	//20回転.
	, m_CurrentTiltAngle	()

	, m_GroundPos			( 0.2f )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerFallingState::~CPlayerFallingState()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerFallingState::Enter()
{
	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetTotalTime();

	//傾き角度の初期化.
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
	m_StartQuat = m_pPlayer.GetQuaternion();

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	//開始時の右軸を設定.
	m_StartRightAxis = axes.right;

	if (m_pPlayer.GetHitAttack().hitEvent == CPlayerBase::HitEvent::None)
	{
		//初速度を設定.
		m_Velocity = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_RotateSpeed = 30.f;
	}
	else
	{
		//初速度を設定.
		m_Velocity = m_pPlayer.GetHitAttack().velocity;
		m_Velocity.y = 0.f;

		//吹き飛ばし量を取得.
		float power = m_pPlayer.GetHitPower();
		m_RotateSpeed += m_ForceMax - power;	//最大量から引いて速さの調整.
	}
}

//--- 状態の終了時に呼び出す ---.
void CPlayerFallingState::Exit()
{
	//SEを鳴らす.
	AssetManager::Sound()->PlaySE(enSoundList::SE_Down);
}

//--- この状態の間に呼び出す ---.
void CPlayerFallingState::Update()
{
	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime
		|| IsEnd())
	{
		//ダウン状態付きの攻撃を受けた場合.
		if(m_pPlayer.GetHitAttack().hitEvent == CPlayerBase::HitEvent::WithDown)
		{
			m_pPlayer.SetActionState(std::make_unique<CPlayerKnockdownState>(m_pPlayer));
			return;
		}
		else
		{
			float downTimeRemaining = m_pPlayer.GetKnockdownTime().remaining;
			//ダウン時間が余っていた場合.
			if (downTimeRemaining > 0.f)
			{
				//残り時間分を設定.
				m_pPlayer.SetHitPower(downTimeRemaining);
				m_pPlayer.SetActionState(std::make_unique<CPlayerKnockdownState>(m_pPlayer));
				return;
			}
			//吹き飛ばす攻撃を受けた場合.
			if (m_pPlayer.GetHitAttack().hitEvent == CPlayerBase::HitEvent::NoDown)
			{
				m_pPlayer.SetActionState(std::make_unique<CPlayerGetUpState>(m_pPlayer));
				return;
			}
			//アイテムを持っている場合.
			if (m_pPlayer.GetHoldingItem())
			{
				//アイテム持ち、何もなし状態.
				m_pPlayer.SetActionState(std::make_unique<CPlayerHoldingIdleState>(m_pPlayer));
				return;
			}
			else
			{
				//何もなし状態.
				m_pPlayer.SetActionState(std::make_unique<CPlayerActionIdleState>(m_pPlayer));
				return;
			}
		}
	}

	//クォータニオンを取得.
	D3DXQUATERNION quat = m_pPlayer.GetQuaternion();

	m_CurrentTiltAngle = m_pPlayer.WrapAngle(m_CurrentTiltAngle);
	//地面近くかつ90度付近の場合.
	if (IsEnd())
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
		m_pPlayer.SetQuaternion(m_pPlayer.TiltedQuat(m_StartQuat, -m_StartRightAxis, m_CurrentTiltAngle));
	}

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	//手の調整位置の設定.
	D3DXVECTOR3 rightHandOffsetPos = m_UpHandOffset + m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffsetPos = m_UpHandOffset + m_pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(rightHandOffsetPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(leftHandOffsetPos));

	//1フレームの速さを取得.
	float dt = CTimeManager::GetDeltaTime();

	//地面についていない場合.
	if (!m_pPlayer.IsOnGround())
	{
		//飛んでいく移動量の計算.
		m_Velocity.y += m_Gravity * dt;
		//プレイヤーに移動量を足す.
		playerPos += m_Velocity * dt;
	}
	//プレイヤーの位置を設定.
	m_pPlayer.SetPosition(playerPos);
}

//======================================================================
// 	   内部で呼び出す関数.
//======================================================================

//--- ワールドでの角度を求める関数 ---.
float CPlayerFallingState::WorldAngle()
{
	//上方向のローカル軸を取得.
	D3DXVECTOR3 localUp = m_pPlayer.GetLocalAxes().up;
	//上方向.
	D3DXVECTOR3 up(0.f, 1.f, 0.f);

	//ローカル軸の上方向と世界軸の上方向の角度差を計算.
	float dot = D3DXVec3Dot(&localUp, &up);
	dot = std::clamp(dot, -1.f, 1.f);
	//角度差の角度を計算.
	float angle = acosf(dot);

	return angle;
}

//--- この状態を終了する関数 ---.
bool CPlayerFallingState::IsEnd()
{
	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();

	//回転を90度付近で止め、位置が地面についた場合.
	if (WorldAngle() > m_RotateRangeMin
		&& WorldAngle() < m_RotateRangeMax
		&& m_pPlayer.IsOnGround())
	{
		return true;
	}

	return false;
}