#include "CPlayerMoveState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"

CPlayerMoveState::CPlayerMoveState(float x, float z)
	: m_Dir					( x, 0.f, z )
	
	, m_CurrentSpeed		()
	, m_MoveSpeed			( 0.15f )
	, m_RotatingMoveSpeed	( 0.1f )
	, m_MoveSpeedScale		( 1.f )

	, m_MoveDir				( 0.f, 0.f, 0.f )
{
}

CPlayerMoveState::~CPlayerMoveState()
{
}

void CPlayerMoveState::Enter(CPlayerBase& pPlayerBase)
{
	pPlayerBase.SetMoving(true);
}

void CPlayerMoveState::Exit(CPlayerBase& pPlayerBase)
{
	pPlayerBase.SetMoving(false);
}

void CPlayerMoveState::Update(CPlayerBase& pPlayerBase)
{
	if (m_Dir.x == 0 && m_Dir.z == 0)
	{
		pPlayerBase.SetMoveState(std::make_unique<CPlayerMoveIdleState>());
		return;
	}

	D3DXVECTOR3 dir(m_Dir.x, 0, m_Dir.z);

	if (D3DXVec3Length(&dir) > 0)
	{
		D3DXVec3Normalize(&m_MoveDir, &dir);
	}
	else
	{
		m_MoveDir = D3DXVECTOR3(0, 0, 0);
	}

	m_CurrentSpeed = m_MoveSpeed;
	if (pPlayerBase.IsRotating())
	{
		m_CurrentSpeed = m_RotatingMoveSpeed;
	}

	D3DXVECTOR3 pos = pPlayerBase.GetPosition();

	D3DXVECTOR3 velocity = m_MoveDir * m_CurrentSpeed;

	pos += velocity;

	pPlayerBase.SetPosition(pos);
}