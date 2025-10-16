#include "CPlayerPushedState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

CPlayerPushedState::CPlayerPushedState()
	: m_StartTime			()
	, m_EndTime				( 0.5f )

	, m_CurrentTiltAngle	()
	, m_TiltAngleMax		( D3DXToRadian( -30.f ) )
	, m_PhaseSplit			( 0.5f )

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

void CPlayerPushedState::Enter(CPlayerBase& pPlayerBase)
{
	pPlayerBase.SetStopping(true);

	//SEを鳴らす.
	AssetManager::Sound()->PlaySE(enSoundList::SE_HitHand);

	//傾き角度の初期化.
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
	m_StartQuat = pPlayerBase.GetQuaternion();

	//攻撃の開始時間を取得.
	m_StartTime = static_cast<float>(CTimeManager::GetTotalTime());

	//手の開始位置の設定.
	m_RightHandStartPos = pPlayerBase.GetPlayerRightHand().GetOffsetPos();
	m_LeftHandStartPos = pPlayerBase.GetPlayerLeftHand().GetOffsetPos();

	//手の開始位置の設定.
	m_RightHandEndPos += m_RightHandStartPos;
	m_LeftHandEndPos += m_LeftHandStartPos;

	//false を入れて、いつでも攻撃が受けられる状態にする
	pPlayerBase.SetHitInfo(false, CPlayerBase::PlayerEvent::None);
}

void CPlayerPushedState::Exit(CPlayerBase& pPlayerBase)
{
	pPlayerBase.SetStopping(false);
}

void CPlayerPushedState::Update(CPlayerBase& pPlayerBase)
{
	pPlayerBase.SetMoveState(std::make_unique<CPlayerMoveIdleState>());
	pPlayerBase.SetTurnState(std::make_unique<CPlayerTurnIdleState>());

	//ゲーム全体の経過時間.
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime)
	{
		pPlayerBase.SetActionState(std::make_unique<CPlayerActionIdleState>());
		return;
	}

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayerBase.GetPosition();
	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = pPlayerBase.GetLocalAxes();
	
	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = pPlayerBase.Clamp(progress, 0.f, 1.f);

	//時間の割合が半分より前なら(倒れる動き).
	if (progress < m_PhaseSplit)
	{
		//倒れきるまでの現在の傾き割合.
		float ratio = progress / m_PhaseSplit;
		//現在の傾き = 最大傾き角度 * 割合.
		m_CurrentTiltAngle = pPlayerBase.WrapAngle(m_TiltAngleMax * ratio);
	}
	//時間の割合が半分以上(戻る動き).
	else if (progress <= 1.0f)
	{
		//傾きの変わり目(m_PhaseSplit)からどれだけ経過したかを割って割合.
		float ratio = (progress - m_PhaseSplit) / m_PhaseSplit;
		//現在の傾き = 最大傾き角度 * (1 - 割合).
		m_CurrentTiltAngle = pPlayerBase.WrapAngle(m_TiltAngleMax * (1.f - ratio));
	}
	else
	{
		//終了後は0度.
		m_CurrentTiltAngle = 0.f;
	}

	//クォータニオンの回転を計算して設定する.
	pPlayerBase.SetQuaternion(pPlayerBase.TiltedQuat(m_StartQuat, axes.right, m_CurrentTiltAngle));

	float eased = cosf(progress * D3DX_PI * 3.f);	//0.5かけて半円分の移動を計算.	

	//手の位置を計算.
	D3DXVECTOR3 rightHandOffsetPos;
	D3DXVec3Lerp(&rightHandOffsetPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	D3DXVECTOR3 leftHandOffsetPos;
	D3DXVec3Lerp(&leftHandOffsetPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//プレイヤーの位置と手の調整位置を合わせる.
	D3DXVECTOR3 rightHandPos = pPlayerBase.GetObjectPos(rightHandOffsetPos);
	D3DXVECTOR3 leftHandPos = pPlayerBase.GetObjectPos(leftHandOffsetPos);

	//手の位置を設定.
	pPlayerBase.GetPlayerRightHand().SetPosition(rightHandPos);
	pPlayerBase.GetPlayerLeftHand().SetPosition(leftHandPos);

	//プレイヤーの位置と押された移動量を足す.
	playerPos += pPlayerBase.Pushed();

	//プレイヤーの位置を設定.
	pPlayerBase.SetPosition(playerPos);
}