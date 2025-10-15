#include "CPlayerGetUpState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

CPlayerGetUpState::CPlayerGetUpState()
	: m_StartTime			()
	, m_EndTime				( 0.5f )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
	, m_DefaultQuat			( 0.f, 0.f, 0.f, 1.f )

	, m_RightHandStartPos	()
	, m_LeftHandStartPos	()
	, m_RightHandEndPos		( 0.f, 1.f, 0.5f )
	, m_LeftHandEndPos		( 0.f, 1.f, 0.5f )
{
}

CPlayerGetUpState::~CPlayerGetUpState()
{
}

void CPlayerGetUpState::Enter(CPlayerBase& pPlayerBase)
{
	//攻撃の開始時間を取得.
	m_StartTime = static_cast<float>(CTimeManager::GetTotalTime());

	//クォータニオン型の回転を取得.
	m_StartQuat = pPlayerBase.GetQuaternion();
	//最終に戻る位置を設定.
	m_DefaultQuat = D3DXQUATERNION(0.f, m_StartQuat.y, 0.f, m_StartQuat.w);

	pPlayerBase.SetQuaternion(m_StartQuat);

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = pPlayerBase.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = pPlayerBase.GetPlayerLeftHand().GetOffsetPos();
	//手の開始位置を設定.
	m_RightHandStartPos = rightHandOffset;
	m_LeftHandStartPos = leftHandOffset;
}

void CPlayerGetUpState::Exit(CPlayerBase& pPlayerBase)
{
	pPlayerBase.SetQuaternion(0.f, m_StartQuat.y, 0.f, m_StartQuat.w);
}

void CPlayerGetUpState::Update(CPlayerBase& pPlayerBase)
{
	pPlayerBase.SetMoveState(std::make_unique<CPlayerMoveIdleState>());
	pPlayerBase.SetTurnState(std::make_unique<CPlayerTurnIdleState>());

	float t = static_cast<float>(CTimeManager::GetTotalTime());

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime)
	{
		pPlayerBase.SetActionState(std::make_unique<CPlayerActionIdleState>());
		return;
	}

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = pPlayerBase.GetLocalAxes();

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = pPlayerBase.Clamp(progress, 0.f, 1.f);

	//滑らかに正常の位置に戻す.
	D3DXQUATERNION quat;
	D3DXQuaternionSlerp(&quat, &m_StartQuat, &m_DefaultQuat, progress);
	pPlayerBase.SetQuaternion(quat);

	if (progress > 0.7f)
	{
		m_RightHandEndPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		m_LeftHandEndPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	}
	else
	{
		m_RightHandEndPos = D3DXVECTOR3(0.f, 0.3f, -0.5f);
		m_LeftHandEndPos = D3DXVECTOR3(0.f, 0.3f, -0.5f);
	}
	float eased = sinf(progress * D3DX_PI * 0.5f);	//それぞれの手の軌道の計算.	

	//手の終了位置を設定.
	m_RightHandEndPos = m_RightHandStartPos + m_RightHandEndPos;
	m_LeftHandEndPos = m_LeftHandStartPos + m_LeftHandEndPos;
	//右手と左手の調整位置だけの計算.
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
}