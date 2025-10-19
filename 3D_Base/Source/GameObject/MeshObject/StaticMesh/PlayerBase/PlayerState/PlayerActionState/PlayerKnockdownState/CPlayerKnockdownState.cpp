#include "CPlayerKnockdownState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerGetUpState/CPlayerGetUpState.h"

CPlayerKnockdownState::CPlayerKnockdownState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )
	
	, m_StartTime			()
	, m_EndTime				( 1.f )

	, m_StartQuat			( 0.f, 0.f, 0.f, 1.f )

	, m_RightHandPos		( -0.1f, 0.3f, 0.1f )
	, m_LeftHandPos			( 0.1f, 0.3f, 0.1f )
{
}

CPlayerKnockdownState::~CPlayerKnockdownState()
{
}

void CPlayerKnockdownState::Enter()
{
	m_pPlayer.SetPlayerEvent(CPlayerBase::PlayerEvent::Knockdown);

	//攻撃の開始時間を取得.
	m_StartTime = static_cast<float>(CTimeManager::GetTotalTime());

	//クォータニオン型の回転を取得.
	m_StartQuat = m_pPlayer.GetQuaternion();
	m_pPlayer.SetQuaternion(m_StartQuat);

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffset = m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffset = m_pPlayer.GetPlayerLeftHand().GetOffsetPos();
	//手の開始位置を設定.
	m_RightHandPos += rightHandOffset;
	m_LeftHandPos += leftHandOffset;

	//m_EndTime = m_pPlayer.GetHitInfo().force * 0.5;
}

void CPlayerKnockdownState::Exit()
{
}

void CPlayerKnockdownState::Update()
{
	float t = static_cast<float>(CTimeManager::GetTotalTime());

	//現在の経過時間と開始時間の差が終了時間を上回ったら.
	if (t - m_StartTime > m_EndTime)
	{
		m_pPlayer.SetActionState(std::make_unique<CPlayerGetUpState>(m_pPlayer));
		return;
	}

	////プレイヤーのローカル軸を取得.
	//CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	////全体の時間の現在の割合.
	//float progress = (t - m_StartTime) / m_EndTime;
	//progress = m_pPlayer.Clamp(progress, 0.f, 1.f);

	////滑らかに正常の位置に戻す.
	//D3DXQUATERNION quat;
	//D3DXQuaternionSlerp(&quat, &m_StartQuat, &m_DefaultQuat, progress);
	////正規化.
	//D3DXQuaternionNormalize(&quat, &quat);
	//m_pPlayer.SetQuaternion(quat);

	//if (progress > 0.7f)
	//{
	//	m_RightHandEndPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	//	m_LeftHandEndPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	//}
	//else
	//{
	//	m_RightHandEndPos = D3DXVECTOR3(0.f, 0.3f, -0.5f);
	//	m_LeftHandEndPos = D3DXVECTOR3(0.f, 0.3f, -0.5f);
	//}
	//float eased = sinf(progress * D3DX_PI * 0.5f);	//それぞれの手の軌道の計算.	

	////手の終了位置を設定.
	//m_RightHandEndPos = m_RightHandStartPos + m_RightHandEndPos;
	//m_LeftHandEndPos = m_LeftHandStartPos + m_LeftHandEndPos;
	////右手と左手の調整位置だけの計算.
	//D3DXVECTOR3 rightHandOffsetPos;
	//D3DXVec3Lerp(&rightHandOffsetPos, &m_RightHandStartPos, &m_RightHandEndPos, eased);
	//D3DXVECTOR3 leftHandOffsetPos;
	//D3DXVec3Lerp(&leftHandOffsetPos, &m_LeftHandStartPos, &m_LeftHandEndPos, eased);

	//手の位置を調整して設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(m_pPlayer.GetObjectPos(m_RightHandPos));
	m_pPlayer.GetPlayerLeftHand().SetPosition(m_pPlayer.GetObjectPos(m_LeftHandPos));
}