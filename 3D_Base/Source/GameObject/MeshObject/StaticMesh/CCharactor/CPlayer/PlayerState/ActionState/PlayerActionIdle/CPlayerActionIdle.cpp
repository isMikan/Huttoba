#include "CPlayerActionIdle.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/ActionState/PlayerHandAttack/CPlayerHandAttack.h"

CPlayerActionIdle::CPlayerActionIdle()
{
}

CPlayerActionIdle::~CPlayerActionIdle()
{
}

void CPlayerActionIdle::Enter(CPlayer& pPlayer)
{
}

void CPlayerActionIdle::Exit(CPlayer& pPlayer)
{
}

void CPlayerActionIdle::Update(CPlayer& pPlayer)
{
	//手の位置を調整するための数値を取得.
	D3DXVECTOR3 m_RightOffset = pPlayer.GetPlayerRightHand().GetOffsetPos();
	D3DXVECTOR3 m_LeftOffset = pPlayer.GetPlayerLeftHand().GetOffsetPos();

	//手の位置計算.
	D3DXVECTOR3 right = pPlayer.HandPositionMath(m_RightOffset);
	D3DXVECTOR3 left = pPlayer.HandPositionMath(m_LeftOffset);

	//手の位置を設定.
	pPlayer.GetPlayerRightHand().SetPosition(right);
	pPlayer.GetPlayerLeftHand().SetPosition(left);
}

void CPlayerActionIdle::Handle(CPlayer& pPlayer, int inputKey)
{
	if (inputKey == 'Z')
	{
		pPlayer.SetActionState(std::make_unique<CPlayerHandAttack>());
	}
}