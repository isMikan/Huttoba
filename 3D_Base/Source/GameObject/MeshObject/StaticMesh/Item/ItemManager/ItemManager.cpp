#include "stdafx.h"
#include "ItemManager.h"
#include "Item/ItemBase.h"	

ItemManager::ItemManager()
	:m_pItems	()
{
	Create();
}

ItemManager::~ItemManager()
{
}

HRESULT ItemManager::Create()
{
	m_pItems.push_back(ItemFactory::GetInstance()->CreateItem("Haetataki"));
	m_pItems.push_back(ItemFactory::GetInstance()->CreateItem("SmashBat"));

	return S_OK;
}

HRESULT ItemManager::LoadData()
{
	return S_OK;
}

void ItemManager::Init()
{
	for (auto& item : m_pItems)
	{
		item->Init();
	}
}

void ItemManager::Update()
{
	for (auto& item : m_pItems)
	{
		item->Update();
	}
}

void ItemManager::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	for (auto& item : m_pItems)
	{
		item->Draw(View,Proj, Light,Camera);
	}
}