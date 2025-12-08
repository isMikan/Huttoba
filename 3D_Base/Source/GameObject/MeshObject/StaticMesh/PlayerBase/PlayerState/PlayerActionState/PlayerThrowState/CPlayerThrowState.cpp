#include "CPlayerThrowState.h"

#include "PlayerBase/CPlayerBase.h"

#include "PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

CPlayerThrowState::CPlayerThrowState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )

	, m_StartTime			()
	, m_EndTime				( 0.4f )

	, m_CurrentTiltAngle	()
	, m_TiltAngleMax		( D3DXToRadian( 30.f ) )
	, m_PhaseSplit			( 0.5f )

	, m_RightHandStartPos	()
	, m_LeftHandStartPos	()
	, m_RightHandEndPos		( 0.f, -0.5f, -0.5f )
	, m_LeftHandEndPos		( 0.f, 0.f, 0.3f )

	, m_StartQuat			()
{
}

CPlayerThrowState::~CPlayerThrowState()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerThrowState::Enter()
{
	//SEを鳴らす.
	AssetManager::Sound()->PlaySE(enSoundList::SE_AttackHand);

	//傾き角度の初期化.
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
	m_StartQuat = m_pPlayer.GetQuaternion();

	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetTotalTime();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = m_pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//手の開始位置を設定.
	m_RightHandStartPos = rightHandOffset;
	m_LeftHandStartPos = leftHandOffset;
	//手の終了位置を設定.
	m_RightHandEndPos = m_RightHandStartPos + m_RightHandEndPos;
	m_LeftHandEndPos = m_LeftHandStartPos + m_LeftHandEndPos;
}

//--- 状態の終了時に呼び出す ---.
void CPlayerThrowState::Exit()
{
	m_pPlayer.SetQuaternion(m_StartQuat);

	m_pPlayer.SetHoldingItem(nullptr);
}

//--- この状態の間に呼び出す ---.
void CPlayerThrowState::Update()
{
	//ゲーム全体の経過時間.
	float t = CTimeManager::GetTotalTime();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime)
	{
		m_pPlayer.SetActionState(std::make_unique<CPlayerActionIdleState>(m_pPlayer));
		return;
	}

	//前回のフレームからの経過時間.
	float dt = CTimeManager::GetDeltaTime();

	//ローカル軸を取得.
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

	float eased = cosf(progress * D3DX_PI);	//それぞれの手の軌道の計算.	

	//右手と左手の調整位置だけの計算.
	D3DXVECTOR3 rightHandOffsetPos;
	D3DXVec3Lerp(&rightHandOffsetPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	D3DXVECTOR3 leftHandOffsetPos;
	D3DXVec3Lerp(&leftHandOffsetPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(rightHandOffsetPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(leftHandOffsetPos));
}