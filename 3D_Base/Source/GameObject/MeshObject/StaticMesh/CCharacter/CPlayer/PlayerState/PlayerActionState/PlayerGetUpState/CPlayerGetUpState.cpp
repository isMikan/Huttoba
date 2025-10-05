#include "CPlayerGetUpState.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"
#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerTurnState/PlayerTurnIdleState/CPlayerTurnIdleState.h"
#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

#include "TimeManager/CTimeManager.h"

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

void CPlayerGetUpState::Enter(CPlayer& pPlayer)
{
	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetInstance()->GetTotalTime();

	//クォータニオン型の回転を取得.
	m_StartQuat = pPlayer.GetQuaternion();
	//最終に戻る位置を設定.
	m_DefaultQuat = D3DXQUATERNION(0.f, m_StartQuat.y, 0.f, m_StartQuat.w);

	pPlayer.SetQuaternion(m_StartQuat);

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = pPlayer.GetPlayerLeftHand().GetOffsetPos();
	//手の開始位置を設定.
	m_RightHandStartPos = rightHandOffset;
	m_LeftHandStartPos = leftHandOffset;
}

void CPlayerGetUpState::Exit(CPlayer& pPlayer)
{
	pPlayer.SetQuaternion(0.f, m_StartQuat.y, 0.f, m_StartQuat.w);
}

void CPlayerGetUpState::Update(CPlayer& pPlayer)
{
	pPlayer.SetMoveState(std::make_unique<CPlayerMoveIdleState>());
	pPlayer.SetTurnState(std::make_unique<CPlayerTurnIdleState>());

	float t = CTimeManager::GetInstance()->GetTotalTime();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime)
	{
		pPlayer.SetActionState(std::make_unique<CPlayerActionIdleState>());
		return;
	}

	//プレイヤーのローカル軸を取得.
	CPlayer::LocalAxes axes = pPlayer.GetLocalAxes();

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = pPlayer.Clamp(progress, 0.f, 1.f);

	//滑らかに正常の位置に戻す.
	D3DXQUATERNION quat;
	D3DXQuaternionSlerp(&quat, &m_StartQuat, &m_DefaultQuat, progress);
	pPlayer.SetQuaternion(quat);

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

	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayer.GetPosition();

	//方向に合わせて右手の位置を調整.
	rightHandOffsetPos =
		axes.right * rightHandOffsetPos.x +
		axes.up * rightHandOffsetPos.y +
		axes.forward * rightHandOffsetPos.z;

	//方向に合わせて左手の位置を調整.
	leftHandOffsetPos =
		axes.right * leftHandOffsetPos.x +
		axes.up * leftHandOffsetPos.y +
		axes.forward * leftHandOffsetPos.z;

	//プレイヤーの位置と手の調整位置を合わせる.
	D3DXVECTOR3 rightHandPos = playerPos + rightHandOffsetPos;
	D3DXVECTOR3 leftHandPos = playerPos + leftHandOffsetPos;

	//手の位置を設定.
	pPlayer.GetPlayerRightHand().SetPosition(rightHandPos);
	pPlayer.GetPlayerLeftHand().SetPosition(leftHandPos);
}