#include "CPlayerMoveState.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/PlayerMoveState/PlayerMoveIdelState/CPlayerMoveIdleState.h"

CPlayerMoveState::CPlayerMoveState(CPlayerBase& pPlayer, float x, float z)
	: CPlayerState			( pPlayer )

	, m_KeyDir				( x, 0.f, z )
	, m_MoveDir				( 0.f, 0.f, 0.f )
	
	, m_CurrentSpeed		()
	, m_MoveSpeed			( 0.14f )
	, m_RotatingMoveSpeed	( 0.1f )
	, m_HitingMoveSpeed		( 0.03f )

{
}

CPlayerMoveState::~CPlayerMoveState()
{
}

void CPlayerMoveState::Enter()
{
	m_pPlayer.SetMoving(true);
}

void CPlayerMoveState::Exit()
{
	m_pPlayer.SetMoving(false);
}

void CPlayerMoveState::Update()
{
	if (m_KeyDir.x == 0 && m_KeyDir.z == 0)
	{
		m_pPlayer.SetMoveState(std::make_unique<CPlayerMoveIdleState>(m_pPlayer));
		return;
	}

	D3DXVECTOR3 dir(m_KeyDir.x, 0, m_KeyDir.z);

	if (D3DXVec3Length(&dir) > 0)
	{
		D3DXVec3Normalize(&m_MoveDir, &dir);
	}
	else
	{
		m_MoveDir = D3DXVECTOR3(0, 0, 0);
	}

	m_CurrentSpeed = m_MoveSpeed;
	if (m_pPlayer.IsRotating())
	{
		m_CurrentSpeed = m_RotatingMoveSpeed;
	}
	if (m_pPlayer.GetPlayerEvent() == CPlayerBase::PlayerEvent::Pushed
		|| m_pPlayer.GetPlayerEvent() == CPlayerBase::PlayerEvent::Falling)
	{
		m_CurrentSpeed = m_HitingMoveSpeed;
	}

	D3DXVECTOR3 pos = m_pPlayer.GetPosition();

	D3DXVECTOR3 velocity = m_MoveDir * m_CurrentSpeed;

	pos += velocity;

	m_pPlayer.SetPosition(pos);
}