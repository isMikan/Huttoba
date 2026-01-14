#include "stdafx.h"
#include "ItemManager.h"
#include "Item/ItemBase.h"	
#include "PlayerBase/PlayerManager/CPlayerManager.h"
#include "Input/CInputManager.h"
#include "Item/SelectSpawnItem/SelectSpawnItem.h"
#include "Item/SpawnItemPosition/SpawnItemPosition.h"


constexpr int INITAL_SPAWN_ITEM_LIMIT = 8;

//--------------------------------------------------------------------------------------------------------------

ItemManager::ItemManager(std::unique_ptr<CGroundManager>& GroundManager)
	: m_pItems				{}
	, m_pSpawnItem			{ std::make_unique<SelectSpawnItem>() }
	, m_pSpawnItemPosition	{ std::make_unique<SpawnItemPosition>(GroundManager) }
	, m_SpawnLimit			{ }

	, m_ErrorCount			{ }
{
	Create();
	Init();
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
	m_SpawnLimit = INITAL_SPAWN_ITEM_LIMIT;
	for (auto& item : m_pItems)
	{
		item->Init();
	}
}

//--------------------------------------------------------------------------------------------------------------

void ItemManager::Update()
{
	m_pSpawnItemPosition->Uptate();

	//アイテム数上限の時は作成しない
	while (m_pItems.size() < m_SpawnLimit)
	{
		//アイテムの作成
		CreateItem();
		m_ErrorCount++;
		//もし無限ループが起きた時の対策
		if (m_ErrorCount >= 100)
		{
			m_pItems.resize(m_SpawnLimit);
			m_ErrorCount = 0;
			_ASSERT_EXPR(false, _T("無限ループ発生"));
		}
	}

	//無限ループではない場合初期化しとく
	m_ErrorCount = 0;

	int itemEffectCount = 0;
	for (auto& item : m_pItems)
	{
		//一旦Player0しか持てないようにする
		item->Update();

		if (!item->GetIsOnGround() && item->GetIsOkFall())
		{
			item->Fall();
		}

		if (AssetManager::Effect()->IsPlaying(item->GetItemOnGroundEffect()))
		{
			itemEffectCount++;
		}

	}

	if (itemEffectCount == 0)
	{
		for (auto& item : m_pItems)
		{
			item->SetIsPlayingItemEffect(true);
		}
	}
	else
	{
		for (auto& item : m_pItems)
		{
			item->SetIsPlayingItemEffect(false);
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
	//アイテム上限数を決定
	CheckSpawnLimit();
		
	//Selectクラスで生成アイテムを選択
	ItemID itemId = m_pSpawnItem->SerectSpawnItem(m_pItems);

	//選択されたアイテム作成
	m_pItems.push_back(ItemFactory::GetInstance()->CreateItem(itemId));
	//m_pItems.push_back(ItemFactory::GetInstance()->CreateItem(ItemID::Mushroom)); //←指定アイテム生成

	//生成されたアイテムの位置設定
	m_pItems.back()->SetPosition(m_pSpawnItemPosition->SerectPosition());
	m_pItems.back()->SetTag(itemId);
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

void ItemManager::CheckSpawnLimit()
{
	//ステージの落ち状況によって上限変更
	switch (m_pSpawnItemPosition->GetCurrentFallGround())
	{
	case GroundTag::SafeGround:			m_SpawnLimit = 0; break;
	case GroundTag::ThirdFallGround:	m_SpawnLimit = 2; break;
	case GroundTag::SecondFallGround:	m_SpawnLimit = 4; break;
	case GroundTag::FirstFallGround:	m_SpawnLimit = 6; break;
	case GroundTag::None:break;
	}
}

//--------------------------------------------------------------------------------------------------------------
