#include "CPlayerHandHitState.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

#include "TimeManager/CTimeManager.h"

CPlayerHandHitState::CPlayerHandHitState()
	: m_StartTime			()
	, m_EndTime				( 0.3f )

	, m_CurrentTiltAngle	()
	, m_TiltAngleMax		( D3DXToRadian( 30.f ) )
	, m_PhaseSplit			( 0.5f )

	, m_RightHandStartPos	( 0.f, 0.f, 0.f )
	, m_LeftHandStartPos	( 0.f, 0.f, 0.f )
	, m_RightHandEndPos		( 0.f, 0.f, 0.f )
	, m_LeftHandEndPos		( 0.f, 0.f, 0.f )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerHandHitState::~CPlayerHandHitState()
{
}

void CPlayerHandHitState::Enter(CPlayer& pPlayer)
{
	//傾き角度の初期化.
	m_CurrentTiltAngle = 0.f;

	//クォータニオン型の回転を取得.
    m_StartQuat = pPlayer.GetQuaternion();

	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetInstance()->GetTotalTime();

	//手の開始位置を設定.
	m_RightHandStartPos = pPlayer.GetPlayerRightHand().GetPosition();
	m_LeftHandStartPos = pPlayer.GetPlayerLeftHand().GetPosition();
}

void CPlayerHandHitState::Exit(CPlayer& pPlayer)
{
}

void CPlayerHandHitState::Update(CPlayer& pPlayer)
{
	//ゲーム全体の経過時間.
	float totalTime = CTimeManager::GetInstance()->GetTotalTime();

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();
	//プレイヤーのローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = pPlayer.GetPlayerLeftHand().GetOffsetPos();
	//プレイヤーの正面方向に手の位置調整.
	rightHandOffset =
		axes.right * rightHandOffset.x +
		axes.up * rightHandOffset.y +
		axes.forward * rightHandOffset.z;
	leftHandOffset = 
		axes.right * leftHandOffset.x +
		axes.up * leftHandOffset.y +
		axes.forward * leftHandOffset.z;
	//手の終了位置を設定.
	m_RightHandEndPos = playerPos + rightHandOffset;
	m_LeftHandEndPos = playerPos + leftHandOffset;

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (totalTime - m_StartTime > m_EndTime
		|| (pPlayer.GetQuaternion().x == 0.f
		&& pPlayer.GetQuaternion().z == 0.f))
	{
		pPlayer.SetActionState(std::make_unique<CPlayerActionIdleState>());
		return;
	}
	
	//全体の時間の現在の割合.
	float progress = (totalTime - m_StartTime) / m_EndTime;

	//現在の傾き = 最大傾き角度 * 割合.
	m_CurrentTiltAngle = m_TiltAngleMax * progress;

	//クォータニオンの回転を計算して設定する.
	pPlayer.SetQuaternion(pPlayer.TiltedQuat(m_StartQuat, -axes.right, m_CurrentTiltAngle));

	float eased = sinf(progress * D3DX_PI * m_PhaseSplit);	//半円分の移動を計算.	

	//プレイヤーの手の位置を計算.
	D3DXVECTOR3 rightHandPos;
	D3DXVec3Lerp(&rightHandPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	D3DXVECTOR3 leftHandPos;
	D3DXVec3Lerp(&leftHandPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//手の位置を設定.
	pPlayer.GetPlayerRightHand().SetPosition(rightHandPos);
	pPlayer.GetPlayerLeftHand().SetPosition(leftHandPos);
}