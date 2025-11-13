#include "stdafx.h"
#include "ItemManager.h"
#include "Item/ItemBase.h"	
#include "PlayerBase/PlayerManager/CPlayerManager.h"
#include "Input/CInputManager.h"


//--------------------------------------------------------------------------------------------------------------

ItemManager::ItemManager()
	:m_pItems	()
{
	Create();
}

//--------------------------------------------------------------------------------------------------------------

ItemManager::~ItemManager()
{
}

//--------------------------------------------------------------------------------------------------------------

HRESULT ItemManager::Create()
{
	m_pItems.push_back(ItemFactory::GetInstance()->CreateItem("Haetataki"));
	//m_pItems.push_back(ItemFactory::GetInstance()->CreateItem("SmashBat"));
	m_pItems.push_back(ItemFactory::GetInstance()->CreateItem("Bomb"));
	m_pItems.push_back(ItemFactory::GetInstance()->CreateItem("Bomb"));
	m_pItems.push_back(ItemFactory::GetInstance()->CreateItem("Fun"));
	m_pItems.push_back(ItemFactory::GetInstance()->CreateItem("Mushroom"));
	//m_pItems.push_back(ItemFactory::GetInstance()->CreateItem("TrackingRobot"));

	for (int i = 0;i < 4;i++)
	{
		CInputManager::BindKey(Action::Have, InputBinding(InputDevice::GamePad, CXInput::A),i);
		CInputManager::BindKey(Action::HaeAttack, InputBinding(InputDevice::GamePad, CXInput::Y),i);
		CInputManager::BindKey(Action::Have, InputBinding(InputDevice::Keyboard, 'Z'),i);
	}

	m_pItems[0]->SetPosition(0, 0, 4);
	m_pItems[1]->SetPosition(-1, 0, 4);

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

HRESULT ItemManager::LoadData()
{
	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

void ItemManager::Init()
{
	for (auto& item : m_pItems)
	{
		item->Init();
	}
}

//--------------------------------------------------------------------------------------------------------------

void ItemManager::Update()
{
	for (auto& item : m_pItems)
	{
		//一旦Player0しか持てないようにする
		item->Update();

		if (CInputManager::IsDown(Action::HaeAttack, 0))
		{
			item->SetState(ItemBase::State::Use);
		}

		if (!item->GetIsOnGround())
		{
			item->Fall();
		}
	}
	//不必要なアイテム削除
	DestroyItem();

}

//--------------------------------------------------------------------------------------------------------------

void ItemManager::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{

	for (auto& item : m_pItems)
	{
		item->Draw(View,Proj, Light,Camera);
	}
}

//--------------------------------------------------------------------------------------------------------------

void ItemManager::DestroyItem()
{
	m_pItems.erase(
		std::remove_if(
			m_pItems.begin(), m_pItems.end(),
			[](const std::unique_ptr<ItemBase>& item)
			{
				return item->IsDestroy();
			}
		),
		m_pItems.end()
	);
}

D3DXVECTOR3 ItemManager::GetItemPos(int i)
{
	return m_pItems[i]->GetPosition();
}

//--------------------------------------------------------------------------------------------------------------
