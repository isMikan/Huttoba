#include "CPlayerActionIdleState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerHandAttackState/CPlayerHandAttackState.h"

CPlayerActionIdleState::CPlayerActionIdleState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )
{
}

CPlayerActionIdleState::~CPlayerActionIdleState()
{
}

void CPlayerActionIdleState::OnNotify(IPlayerObserver::PlayerEvent event)
{
}

void CPlayerActionIdleState::Enter()
{
	m_pPlayer.SetQuaternion(
		0.f, m_pPlayer.GetQuaternion().y, 0.f, m_pPlayer.GetQuaternion().w);
}

void CPlayerActionIdleState::Exit()
{
}

void CPlayerActionIdleState::Update()
{
	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = m_pPlayer.GetPosition();

	//ローカル軸を取得.
	CPlayerBase::LocalAxes axes = m_pPlayer.GetLocalAxes();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffsetPos = m_pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffsetPos = m_pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//プレイヤーの位置と手の調整位置を合わせる.
	D3DXVECTOR3 rightHandPos = m_pPlayer.GetObjectPos(rightHandOffsetPos);
	D3DXVECTOR3 leftHandPos = m_pPlayer.GetObjectPos(leftHandOffsetPos);

	//手の位置を設定.
	m_pPlayer.GetPlayerRightHand().SetPosition(rightHandPos);
	m_pPlayer.GetPlayerLeftHand().SetPosition(leftHandPos);
}
