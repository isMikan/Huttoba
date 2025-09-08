#include "stdafx.h"
#include "ItemManager.h"
#include "Item/ItemBase.h"	
#include "CCharactor/CPlayer/CPlayer.h"
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

	CInputManager::Instance().BindKey(Action::Have, InputBinding(InputDevice::GamePad, CXInput::A));
	CInputManager::Instance().BindKey(Action::HaeAttack, InputBinding(InputDevice::GamePad, CXInput::X));
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
		//ˆê’UPlayer0‚µ‚©Ž‚Ä‚È‚¢‚æ‚¤‚É‚·‚é
		item->Update(players);

		CInputManager::Instance().Update();

		for (auto& player : players)
		{
			// “–‚½‚è”»’è
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
