#include "CPlayerGetUpState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerActionIdleState/CPlayerActionIdleState.h"

CPlayerGetUpState::CPlayerGetUpState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )
	
	, m_StartTime			()
	, m_EndTime				( 0.5f )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )
	, m_DefaultQuat			( 0.f, 0.f, 0.f, 1.f )

	, m_RightHandStartPos	( -0.1f, 0.1f, 0.1f )
	, m_LeftHandStartPos	(  0.1f, 0.1f, 0.1f )
	, m_RightHandEndPos		( -0.1f, 0.2f, 0.5f )
	, m_LeftHandEndPos		(  0.1f, 0.2f, 0.5f )
{
}

CPlayerGetUpState::~CPlayerGetUpState()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 状態の開始時に呼び出す ---.
void CPlayerGetUpState::Enter()
{
	//攻撃の開始時間を取得.
	m_StartTime = CTimeManager::GetTotalTime();

	//クォータニオン型の回転を取得.
	m_StartQuat = m_pPlayer.GetQuaternion();
	//最終に戻る位置を設定.
	m_DefaultQuat = D3DXQUATERNION(0.f, m_StartQuat.y, 0.f, m_StartQuat.w);

	m_pPlayer.SetQuaternion(m_StartQuat);

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = m_pPlayer.GetPlayerLeftHand().GetOffsetPos();
	//手の開始位置を設定.
	m_RightHandStartPos += rightHandOffset;
	m_LeftHandStartPos += leftHandOffset;
	//手の終了位置を設定.
	m_RightHandEndPos += m_RightHandStartPos;
	m_LeftHandEndPos += m_LeftHandStartPos;
}

//--- 状態の終了時に呼び出す ---.
void CPlayerGetUpState::Exit()
{
	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();
	//地面に着地.
	m_pPlayer.SetPosition(playerPos.x, 0.f, playerPos.z);
}

//--- この状態の間に呼び出す ---.
void CPlayerGetUpState::Update()
{
	float t = CTimeManager::GetTotalTime();

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime)
	{
		m_pPlayer.SetActionState(std::make_unique<CPlayerActionIdleState>(m_pPlayer));
		return;
	}

	//プレイヤーのローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = std::clamp(progress, 0.f, 1.f);

	//滑らかに正常の位置に戻す.
	D3DXQUATERNION quat;
	D3DXQuaternionSlerp(&quat, &m_StartQuat, &m_DefaultQuat, progress);
	//正規化.
	D3DXQuaternionNormalize(&quat, &quat);
	m_pPlayer.SetQuaternion(quat);

	float eased = sinf(progress * D3DX_PI);	//それぞれの手の軌道の計算.	

	//右手と左手の調整位置だけの計算.
	D3DXVECTOR3 rightHandOffsetPos;
	D3DXVec3Lerp(&rightHandOffsetPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	D3DXVECTOR3 leftHandOffsetPos;
	D3DXVec3Lerp(&leftHandOffsetPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(rightHandOffsetPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(leftHandOffsetPos));
}