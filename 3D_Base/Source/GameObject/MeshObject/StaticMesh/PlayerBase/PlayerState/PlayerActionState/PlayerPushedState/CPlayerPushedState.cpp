#include "CPlayerPushedState.h"

#include "PlayerBase/CPlayerBase.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"
#include "PlayerBase/PlayerState/PlayerActionState/PlayerHoldingIdleState/CPlayerHoldingIdleState.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerKnockdownState/CPlayerKnockdownState.h"

CPlayerPushedState::CPlayerPushedState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )

	, m_StartTime			()
	, m_EndTime				( 0.5f )

	, m_CurrentTiltAngle	()
	, m_TiltAngleMax		( D3DXToRadian( -30.f ) )
	, m_PhaseSplit			( 0.5f )
	, m_HandLaps			( 3.f )	//3周.

	, m_RightHandStartPos	( 0.f, 0.f, 0.f )
	, m_LeftHandStartPos	( 0.f, 0.f, 0.f )
	, m_RightHandEndPos		( 0.f, 0.2f, 0.1f )
	, m_LeftHandEndPos		( 0.f, 0.2f, 0.1f )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerPushedState::~CPlayerPushedState()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerPushedState::Enter()
{
	//SEを鳴らす.
	AssetManager::Sound()->PlaySE(enSoundList::SE_HitHand);

	//傾き角度の初期化.
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
	m_StartQuat = m_pPlayer.GetQuaternion();

	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetTotalTime();

	//手の開始位置の設定.
	m_RightHandStartPos = m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	m_LeftHandStartPos = m_pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//手の開始位置の設定.
	m_RightHandEndPos += m_RightHandStartPos;
	m_LeftHandEndPos += m_LeftHandStartPos;

	//false を入れて、いつでも攻撃が受けられる状態にする
	m_pPlayer.SetHitAnim(CPlayerBase::HitEvent::None);
}

//--- 状態の終了時に呼び出す ---.
void CPlayerPushedState::Exit()
{
}

//--- この状態の間に呼び出す ---.
void CPlayerPushedState::Update()
{
	//ゲーム全体の経過時間.
	float t = CTimeManager::GetTotalTime();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime)
	{
		float downTimeRemaining = m_pPlayer.GetKnockdownTime().remaining;
		//ダウン時間が余っていた場合.
		if (downTimeRemaining > 0.f)
		{
			//残り時間分を設定.
			m_pPlayer.SetHitPower(downTimeRemaining);
			m_pPlayer.SetActionState(std::make_unique<CPlayerKnockdownState>(m_pPlayer));
		}
		else
		{
			//アイテムを持っている場合.
			if (m_pPlayer.GetItemBase())
			{
				//アイテム持ち、何もなし状態.
				m_pPlayer.SetActionState(std::make_unique<CPlayerHoldingIdleState>(m_pPlayer));
			}
			else
			{
				//何もなし状態.
				m_pPlayer.SetActionState(std::make_unique<CPlayerActionIdleState>(m_pPlayer));
			}
		}
		return;
	}

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();
	
	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = std::clamp(progress, 0.f, 1.f);

	//時間の割合が半分より前なら(倒れる動き).
	if (progress < m_PhaseSplit)
	{
		//倒れきるまでの現在の傾き割合.
		float ratio = progress / m_PhaseSplit;
		//現在の傾き = 最大傾き角度 * 割合.
		m_CurrentTiltAngle = m_pPlayer.WrapAngle(m_TiltAngleMax * ratio);
	}
	//時間の割合が半分以上(戻る動き).
	else if (progress <= 1.0f)
	{
		//傾きの変わり目(m_PhaseSplit)からどれだけ経過したかを割って割合.
		float ratio = (progress - m_PhaseSplit) / m_PhaseSplit;
		//現在の傾き = 最大傾き角度 * (1 - 割合).
		m_CurrentTiltAngle = m_pPlayer.WrapAngle(m_TiltAngleMax * (1.f - ratio));
	}
	else
	{
		//終了後は0度.
		m_CurrentTiltAngle = 0.f;
	}

	//クォータニオンの回転を計算して設定する.
	m_pPlayer.SetQuaternion(m_pPlayer.TiltedQuat(m_StartQuat, axes.right, m_CurrentTiltAngle));

	//手の軌道を計算.
	float eased = cosf(progress * D3DX_PI * m_HandLaps);	

	//手の位置を計算.
	D3DXVECTOR3 rightHandOffsetPos;
	D3DXVec3Lerp(&rightHandOffsetPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	D3DXVECTOR3 leftHandOffsetPos;
	D3DXVec3Lerp(&leftHandOffsetPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(rightHandOffsetPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(leftHandOffsetPos));

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();

	//プレイヤーの位置と押された移動量を足す.
	playerPos += m_pPlayer.GetHitAttack().velocity;

	//プレイヤーの位置を設定.
	m_pPlayer.SetPosition(playerPos);
}