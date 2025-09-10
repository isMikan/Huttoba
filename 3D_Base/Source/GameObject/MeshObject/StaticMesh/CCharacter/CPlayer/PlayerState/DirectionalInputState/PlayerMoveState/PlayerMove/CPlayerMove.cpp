#include "CPlayerMove.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/CPlayer.h"

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/DirectionalInputState/PlayerMoveState/PlayerIdle/CPlayerMoveIdle.h"

CPlayerMove::CPlayerMove()
	: m_CurrentSpeed		()
	, m_MoveSpeed			( 0.15f )
	, m_RotatingMoveSpeed	( 0.1f )
	, m_MoveSpeedScale		( 1.f )
	, m_MoveDir				( 0, 0, 0 )
{
}

CPlayerMove::~CPlayerMove()
{
}

void CPlayerMove::Enter(CPlayer& pPlayer)
{
	pPlayer.SetMoving(true);
}

void CPlayerMove::Exit(CPlayer& pPlayer)
{
	pPlayer.SetMoving(false);
}

void CPlayerMove::Update(CPlayer& pPlayer)
{
	m_CurrentSpeed = m_MoveSpeed;
	if (pPlayer.IsRotating())
	{
		m_CurrentSpeed = m_RotatingMoveSpeed;
	}

	D3DXVECTOR3 pos = pPlayer.GetPosition();

	D3DXVECTOR3 velocity = m_MoveDir * m_CurrentSpeed;

	pos += velocity;

	pPlayer.SetPosition(pos);
}

void CPlayerMove::KeyInput(CPlayer& pPlayer, float x, float z)
{
	if (x == 0 && z == 0)
	{
		pPlayer.SetMoveState(std::make_unique<CPlayerMoveIdle>());
		return;
	}

	D3DXVECTOR3 dir(x, 0, z);

	if (D3DXVec3Length(&dir) > 0)
	{
		D3DXVec3Normalize(&m_MoveDir, &dir);
	}
	else
	{
		m_MoveDir = D3DXVECTOR3(0, 0, 0);
	}
}
