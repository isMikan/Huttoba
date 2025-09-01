#include "stdafx.h"
#include "ItemBase.h"


ItemBase::ItemBase()
	: m_Item()
{
	m_Item = std::make_unique<CStaticMeshObject>();
}

ItemBase::~ItemBase()
{
}

void ItemBase::Init()
{
}

void ItemBase::Update()
{
}

void ItemBase::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{

}