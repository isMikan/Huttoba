#include "stdafx.h"
#include "Bomb.h"

//Factory‚É“o˜^
namespace { const bool regist = ItemBase::AutoRegister<Bomb>("Bomb"); }

Bomb::Bomb()
{
	Init();
}

Bomb::~Bomb()
{
}

void Bomb::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Bomb));
	CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::Bomb));

	SetPosition(1, 5, 2);

	m_State = ItemBase::State::Spawn;

	m_tGravity = 0.01;
}

void Bomb::Update(std::vector<std::unique_ptr<CPlayerBase>>& playiers)
{
	ItemBase::Update(playiers);
}

void Bomb::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	ItemBase::Draw(View, Proj, Light, Camera);
}

void Bomb::Spawn()
{
	//—Ž‰ºˆ—
	if (m_vPosition.y > 1.2)
	{
		m_vPosition.y -= m_tGravity;
		m_tGravity += 0.001f;
	}
	else
	{
		m_State = ItemBase::State::OnGround;
	}
}

void Bomb::OnGround()
{
}

void Bomb::Have(std::vector<std::unique_ptr<CPlayerBase>>& playiers)
{
}

void Bomb::Use(std::vector<std::unique_ptr<CPlayerBase>>& playiers)
{
}

void Bomb::Throw()
{
}

void Bomb::Destroy()
{
}
