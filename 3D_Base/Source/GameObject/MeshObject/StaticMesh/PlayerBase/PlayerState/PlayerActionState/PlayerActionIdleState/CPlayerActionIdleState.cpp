#include "CPlayerActionIdleState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerActionState/PlayerHandAttackState/CPlayerHandAttackState.h"

CPlayerActionIdleState::CPlayerActionIdleState()
{
}

CPlayerActionIdleState::~CPlayerActionIdleState()
{
}

void CPlayerActionIdleState::Enter(CPlayerBase& pPlayerBase)
{
	pPlayerBase.SetQuaternion(
		0.f, pPlayerBase.GetQuaternion().y, 0.f, pPlayerBase.GetQuaternion().w);
}

void CPlayerActionIdleState::Exit(CPlayerBase& pPlayerBase)
{
}

void CPlayerActionIdleState::Update(CPlayerBase& pPlayerBase)
{
	//プレイヤーの位置を取得.
	D3DXVECTOR3 playerPos = pPlayerBase.GetPosition();

	//ローカル軸を取得.
	CPlayerBase::LocalAxes axes = pPlayerBase.GetLocalAxes();

	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 rightHandOffsetPos = pPlayerBase.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 leftHandOffsetPos = pPlayerBase.GetPlayerLeftHand().GetOffsetPos();

	//プレイヤーの位置と手の調整位置を合わせる.
	D3DXVECTOR3 rightHandPos = pPlayerBase.GetObjectPos(rightHandOffsetPos);
	D3DXVECTOR3 leftHandPos = pPlayerBase.GetObjectPos(leftHandOffsetPos);

	//手の位置を設定.
	pPlayerBase.GetPlayerRightHand().SetPosition(rightHandPos);
	pPlayerBase.GetPlayerLeftHand().SetPosition(leftHandPos);
}
