#include "CPlayerHandHitState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

CPlayerHandHitState::CPlayerHandHitState()
	: m_StartTime			()
	, m_EndTime				( 0.3f )

	, m_PhaseSplit			( 0.5f )

	, m_RightHandStartPos	( 0.f, 0.f, 0.f )
	, m_LeftHandStartPos	( 0.f, 0.f, 0.f )
	, m_RightHandEndPos		( 0.f, 0.f, 0.f )
	, m_LeftHandEndPos		( 0.f, 0.f, 0.f )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
	, m_DefaultQuat			( 0.f, 0.f, 0.f, 1.f )
{
}

CPlayerHandHitState::~CPlayerHandHitState()
{
}

void CPlayerHandHitState::Enter(CPlayerBase& pPlayerBase)
{
	//クォータニオン型の回転を取得.
    m_StartQuat = pPlayerBase.GetQuaternion();

	//攻撃の開始時間を取得.
	m_StartTime = static_cast<float>(CTimeManager::GetTotalTime());
	//最終に戻る位置を設定.
	m_DefaultQuat = D3DXQUATERNION(0.f, m_StartQuat.y, 0.f, m_StartQuat.w);

	//手の開始位置を設定.
	m_RightHandStartPos = pPlayerBase.GetPlayerRightHand().GetPosition();
	m_LeftHandStartPos = pPlayerBase.GetPlayerLeftHand().GetPosition();
}

void CPlayerHandHitState::Exit(CPlayerBase& pPlayerBase)
{
	pPlayerBase.SetQuaternion(0.f, m_StartQuat.y, 0.f, m_StartQuat.w);
}

void CPlayerHandHitState::Update(CPlayerBase& pPlayerBase)
{
	//ゲーム全体の経過時間.
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime
		|| (pPlayerBase.GetQuaternion().x == 0.f
		&& pPlayerBase.GetQuaternion().z == 0.f))
	{
		pPlayerBase.SetActionState(std::make_unique<CPlayerActionIdleState>());
		return;
	}
	
	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = pPlayerBase.Clamp(progress, 0.f, 1.f);

	//滑らかに正常の位置に戻す.
	D3DXQUATERNION quat;
	D3DXQuaternionSlerp(&quat, &m_StartQuat, &m_DefaultQuat, progress);
	pPlayerBase.SetQuaternion(quat);

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayerBase.GetPosition();
	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = pPlayerBase.GetLocalAxes();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = pPlayerBase.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = pPlayerBase.GetPlayerLeftHand().GetOffsetPos();
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

	float eased = sinf(progress * D3DX_PI * m_PhaseSplit);	//半円分の移動を計算.	

	//プレイヤーの手の位置を計算.
	D3DXVECTOR3 rightHandPos;
	D3DXVec3Lerp(&rightHandPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	D3DXVECTOR3 leftHandPos;
	D3DXVec3Lerp(&leftHandPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//手の位置を設定.
	pPlayerBase.GetPlayerRightHand().SetPosition(rightHandPos);
	pPlayerBase.GetPlayerLeftHand().SetPosition(leftHandPos);
}