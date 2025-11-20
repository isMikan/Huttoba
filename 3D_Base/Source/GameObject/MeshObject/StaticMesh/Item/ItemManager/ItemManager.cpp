#include "stdafx.h"
#include "ItemManager.h"
#include "Item/ItemBase.h"	
#include "PlayerBase/PlayerManager/CPlayerManager.h"
#include "Input/CInputManager.h"
#include "Item/SelectSpawnItem/SelectSpawnItem.h"
#include "Item/SpawnItemPosition/SpawnItemPosition.h"


//--------------------------------------------------------------------------------------------------------------

ItemManager::ItemManager(std::unique_ptr<CGroundManager>& GroundManager)
	: m_pItems				{}
	, m_pSpawnItem			{ std::make_unique<SelectSpawnItem>() }
	, m_pSpawnItemPosition	{ std::make_unique<SpawnItemPosition>(GroundManager) }
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
	for (int i = 0;i < 4;i++)
	{
		CInputManager::BindKey(Action::Have, InputBinding(InputDevice::GamePad, CXInput::A),i);
		CInputManager::BindKey(Action::HaeAttack, InputBinding(InputDevice::GamePad, CXInput::B),i);
		CInputManager::BindKey(Action::Have, InputBinding(InputDevice::Keyboard, 'Z'),i);
	}

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
	m_pSpawnItemPosition->Uptate();

	//アイテムの作成
	CreateItem();

	for (auto& item : m_pItems)
	{
		//一旦Player0しか持てないようにする
		item->Update();

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

void ItemManager::CreateItem()
{
	//アイテム数上限の時は作成しない
	if (SPAWN_LIMIT < m_pItems.size()) return;
		
	//Selectクラスで生成アイテムを選択
	ItemID itemId = m_pSpawnItem->SerectSpawnItem(m_pItems);

	//選択されたアイテム作成
	m_pItems.push_back(ItemFactory::GetInstance()->CreateItem(ItemID::Bomb));
	//m_pItems.push_back(ItemFactory::GetInstance()->CreateItem(ItemID::TrackingRobot));

	//生成されたアイテムの位置設定
	m_pItems.back()->SetPosition(m_pSpawnItemPosition->SerectPosition());
}

//--------------------------------------------------------------------------------------------------------------

void ItemManager::DestroyItem()
{
	//各アイテムが持っているbool型のIsDestoroyを見てtrueだったら削除
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
