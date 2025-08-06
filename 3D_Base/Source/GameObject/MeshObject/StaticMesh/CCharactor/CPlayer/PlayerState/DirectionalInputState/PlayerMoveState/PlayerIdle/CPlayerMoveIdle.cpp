#include "CPlayerMoveIdle.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/DirectionalInputState/PlayerMoveState/PlayerMove/CPlayerMove.h"

CPlayerMoveIdle::CPlayerMoveIdle()
{
}

CPlayerMoveIdle::~CPlayerMoveIdle()
{
}

void CPlayerMoveIdle::Enter(CPlayer& pPlayer)
{
}

void CPlayerMoveIdle::Exit(CPlayer& pPlayer)
{
}

void CPlayerMoveIdle::Update(CPlayer& pPlayer)
{
}

std::string CPlayerMoveIdle::GetStateName() const
{
	return "MoveIdle";
}

void CPlayerMoveIdle::KeyInput(CPlayer& pPlayer, float x, float z)
{
	if (x != 0 || z != 0)
	{
		pPlayer.SetMoveState(std::make_unique<CPlayerMove>());
	}
}
