#include "CPlayerRotationIdle.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/DirectionalInputState/PlayerRotationState/PlayerRotation/CPlayerRotation.h"

CPlayerRotationIdle::CPlayerRotationIdle()
{
}

CPlayerRotationIdle::~CPlayerRotationIdle()
{
}

void CPlayerRotationIdle::Enter(CPlayer& pPlayer)
{
}

void CPlayerRotationIdle::Exit(CPlayer& pPlayer)
{
}

void CPlayerRotationIdle::Update(CPlayer& pPlayer)
{
}

void CPlayerRotationIdle::KeyInput(CPlayer& pPlayer, float x, float z)
{
	if (x != 0 || z != 0)
	{
		pPlayer.SetRotationState(std::make_unique<CPlayerRotation>());
	}
}
