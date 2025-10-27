#include "CPlayerMoveIdleState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

CPlayerMoveIdleState::CPlayerMoveIdleState(CPlayerBase& pPlayer)
	: CPlayerState			( pPlayer )
{
}

CPlayerMoveIdleState::CPlayerMoveIdleState(CPlayerBase& pPlayer, float x, float z)
	: CPlayerState			( pPlayer )
{
}

CPlayerMoveIdleState::~CPlayerMoveIdleState()
{
}
