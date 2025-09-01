#include "stdafx.h"
#include "Haetataki.h"

//--------------------------------------------------------------------------------------------------------------

//Factory‚É“o˜^
namespace { const bool regist = ItemBase::AutoRegister<Haetataki>("Haetataki"); }

//--------------------------------------------------------------------------------------------------------------

Haetataki::Haetataki()
{
	m_Item->AttachMesh(AssetManager::Mesh(StaticMeshList::BSphere));
	m_Item->CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::BSphere));
}

//--------------------------------------------------------------------------------------------------------------

Haetataki::~Haetataki()
{
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Init()
{
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Update()
{
	m_Item->UpdateBSpherePos();
	auto pos = m_Item->GetPosition();
	pos.x += 0.01;
	m_Item->SetPosition(pos);
}

//--------------------------------------------------------------------------------------------------------------

void Haetataki::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	m_Item->Draw(View, Proj, Light, Camera);
}

//--------------------------------------------------------------------------------------------------------------

