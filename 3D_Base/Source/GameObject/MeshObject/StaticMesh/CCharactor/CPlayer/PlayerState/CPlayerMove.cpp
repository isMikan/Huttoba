#include "CPlayerMove.h"

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"

#include "CPlayerMoveIdle.h"

CPlayerMove::CPlayerMove()
	: m_MoveSpeed		( 0.05f )
	, m_MoveSpeedScale	( 1.f )
	, m_MoveDir			( 0, 0, 0 )
	, m_MoveState		( MoveNone )
{
}

CPlayerMove::~CPlayerMove()
{
}

void CPlayerMove::Enter(CPlayer& pPlayer)
{
}

void CPlayerMove::Eixt(CPlayer& pPlayer)
{
}

void CPlayerMove::Update(CPlayer& pPlayer)
{
	D3DXVECTOR3 pos = pPlayer.GetPosition();

	D3DXVECTOR3 velocity = m_MoveDir * m_MoveSpeed;

	pos += velocity;

#if 1
#else
	//ˆÚ“®ó‘Ô‚É‚æ‚Á‚Äˆ—‚ð•ª‚¯‚é.
	switch (m_MoveState)
	{
	case MoveForward:	//‘O.
		pos.z += m_MoveSpeed;
		break;
	case MoveBackward:	//Œã.
		pos.z -= m_MoveSpeed;
		break;
	case MoveLeft:		//¶.
		pos.x -= m_MoveSpeed;
		break;
	case MoveRight:		//‰E.
		pos.x += m_MoveSpeed;
		break;
	default:
		break;
	}
#endif

	pPlayer.SetPosition(pos);
}

void CPlayerMove::Handle(CPlayer& pPlayer, int inputKey)
{
}

std::string CPlayerMove::GetStateName() const
{
	return "Move";
}

void CPlayerMove::KeyInput(CPlayer& pPlayer, float x, float z)
{
	if (x == 0 && z == 0)
	{
		pPlayer.SetMoveState(new CPlayerMoveIdle);
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
