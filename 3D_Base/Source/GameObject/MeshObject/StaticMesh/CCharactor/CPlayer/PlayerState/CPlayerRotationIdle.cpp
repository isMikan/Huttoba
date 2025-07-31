#include "CPlayerRotationIdle.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"

#include "CPlayerRotation.h"

CPlayerRotationIdle::CPlayerRotationIdle()
{
}

CPlayerRotationIdle::~CPlayerRotationIdle()
{
}

void CPlayerRotationIdle::Enter(CPlayer& pPlayer)
{
}

void CPlayerRotationIdle::Eixt(CPlayer& pPlayer)
{
}

void CPlayerRotationIdle::Update(CPlayer& pPlayer)
{
}

void CPlayerRotationIdle::Handle(CPlayer& pPlayer, int inputKey)
{
	switch (inputKey)
	{
	case VK_UP:		//ëO.
	case VK_DOWN:	//å„.
	case VK_LEFT:	//ç∂.
	case VK_RIGHT:	//âE.
		break;
	default:
		break;
	}
}

void CPlayerRotationIdle::KeyInput(CPlayer& pPlayer, float x, float z)
{
	if (x != 0 || z != 0)
	{
		pPlayer.SetRotationState(std::make_unique<CPlayerRotation>());
	}
}

std::string CPlayerRotationIdle::GetStateName() const
{
	return "RotationIdle";
}
