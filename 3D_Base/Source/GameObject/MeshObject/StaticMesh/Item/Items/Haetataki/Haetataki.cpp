#include "stdafx.h"
#include "Haetataki.h"

//--------------------------------------------------------------------------------------------------------------

//Factoryに登録
namespace { const bool regist = ItemBase::AutoRegister<Haetataki>("Haetataki"); }

//--------------------------------------------------------------------------------------------------------------

Haetataki::Haetataki()
{
	Init();
}

//--------------------------------------------------------------------------------------------------------------

Haetataki::~Haetataki()
{
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Update(CPlayer* player)
{
	//アイテム共通のUpdate
	ItemBase::Update(player);
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	//アイテム共通のDraw
	ItemBase::Draw(View, Proj, Light, Camera);
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Init()
{
	AttachMesh(AssetManager::Mesh(StaticMeshList::Haetataki));
	CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::BSphere));
	SetPosition(0.f, 5.f, 5.f);

	m_State = ItemBase::State::Spawn;
	m_tGravity = 0.01f;
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Spawn()
{
	//落下処理
	if(m_vPosition.y > 0.7f)
	{
		m_vPosition.y -= m_tGravity;
		m_tGravity += 0.001f;
	}
	else
	{
		m_State = ItemBase::State::OnGround;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::OnGround()
{
	if (GetAsyncKeyState('N') & 0x8000)
	{
		m_State = ItemBase::State::Have;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Have()
{
	

	if (GetAsyncKeyState('N') & 0x8000)
	{
		m_State = ItemBase::State::Use;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Use()
{
	if (GetAsyncKeyState('N') & 0x8000)
	{
		m_State = ItemBase::State::Throw;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Throw()
{
	if (GetAsyncKeyState('N') & 0x8000)
	{
		m_State = ItemBase::State::Destroy;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Destroy()
{
	if (GetAsyncKeyState('N') & 0x8000)
	{
		m_State = ItemBase::State::Spawn;
	}
}

//--------------------------------------------------------------------------------------------------------------

