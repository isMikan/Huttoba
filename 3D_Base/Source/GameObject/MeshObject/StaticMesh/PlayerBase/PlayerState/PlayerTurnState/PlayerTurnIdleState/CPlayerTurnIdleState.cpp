#include "CPlayerTurnIdleState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

CPlayerTurnIdleState::CPlayerTurnIdleState(CPlayerBase& pPlayer)
	: CPlayerState		( pPlayer )
{
}

CPlayerTurnIdleState::CPlayerTurnIdleState(CPlayerBase& pPlayer, float x, float z)
	: CPlayerState			( pPlayer )
{
}

CPlayerTurnIdleState::~CPlayerTurnIdleState()
{
}

