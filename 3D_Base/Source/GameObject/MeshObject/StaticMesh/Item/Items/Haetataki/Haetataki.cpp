#include "stdafx.h"
#include "Haetataki.h"
#include "CCharactor/CPlayer/CPlayer.h"

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
	SetRotation(0.f,0.,90.f);
	m_State = ItemBase::State::Spawn;
	m_tGravity = 0.01f;
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Spawn()
{
	//落下処理
	if(m_vPosition.y > 1.2f)
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

void Haetataki::Have(CPlayer* player)
{
	//プレイヤーの位置に合わせるためのオフセット
	D3DXVECTOR3 offset = { 0.5f, 1.f, 0.f };

	//アイテムを拾うモーション
	TakeMostion();

	//アイテムをプレイヤーの位置に合わせる
	m_vPosition = player->GetPosition() + offset;

	if (GetAsyncKeyState('N') & 0x0001)
	{
		m_State = ItemBase::State::Use;
	}
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Use(CPlayer* player)
{
	static D3DXVECTOR3 offset = { 0.5f, 1.f, 0.f };
	static D3DXVECTOR3 addPos = { 0.f,0.f, 0.f };
	static D3DXVECTOR3 addRot = { 0.2f,0.2f,0.f };
	static D3DXVECTOR3 newRot = GetRotation() + addRot;
	static bool switchDir = false;

	

	m_vPosition = player->GetPosition() + offset ;

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

void Haetataki::TakeMostion()
{
	static D3DXVECTOR3 addRot = { 0.2f,0.4f,0.f };

	//所持モーション
	if (m_vRotation.y < D3DXToRadian(145))
	{
		m_vRotation.y += addRot.y;
	}
	if (m_vRotation.x < D3DXToRadian(90))
	{
		m_vRotation.x += addRot.x;
	}
}

//--------------------------------------------------------------------------------------------------------------

