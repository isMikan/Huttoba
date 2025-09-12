#include "stdafx.h"
#include "ItemManager.h"
#include "Item/ItemBase.h"	
#include "CCharacter/CPlayer/CPlayer.h"
#include "CInput/CInputManager.h"


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
	m_pItems.push_back(ItemFactory::GetInstance()->CreateItem("SmashBat"));
	m_pItems.push_back(ItemFactory::GetInstance()->CreateItem("Bomb"));

	CInputManager::Instance().BindKey(Action::Have, InputBinding(InputDevice::GamePad, CXInput::A));
	CInputManager::Instance().BindKey(Action::HaeAttack, InputBinding(InputDevice::GamePad, CXInput::Y));
	CInputManager::Instance().BindKey(Action::Have, InputBinding(InputDevice::Keyboard, 'Z'));

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

void ItemManager::Update(std::vector<std::unique_ptr<CPlayer>>& players)
{
	for (auto& item : m_pItems)
	{
		//一旦Player0しか持てないようにする
		item->Update(players);

		CInputManager::Instance().Update();

		for (auto& player : players)
		{
			// 当たり判定
			if (item->GetBSphere()->IsHit(*player->GetBSphere()))
			{
				if (CInputManager::Instance().GetInput(0).IsDown(Action::Have))
				{
					item->SetState(ItemBase::State::Have);
				}
			}
			if (CInputManager::Instance().GetInput(0).IsDown(Action::HaeAttack))
			{
				item->SetState(ItemBase::State::Use);
			}
		}
	}

	//不必要なアイテム削除
	DestroyItem();
}

//--------------------------------------------------------------------------------------------------------------

void ItemManager::Update()
{

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

//--------------------------------------------------------------------------------------------------------------
