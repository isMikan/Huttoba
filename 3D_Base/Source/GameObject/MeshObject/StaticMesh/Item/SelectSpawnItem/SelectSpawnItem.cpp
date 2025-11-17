#include "stdafx.h"
#include "SelectSpawnItem.h"
#include "Item/ItemBase.h"
#include "Item/ItemFactory/ItemFactory.h"


SelectSpawnItem::SelectSpawnItem()
	: m_ItemsSpawnCount	{}
	, m_IsSpawnMagnet		{}
	, m_MinItemCount		{}

{
	//factoryから登録済アイテムの一覧をもらい、登録されているアイテム分のカウントを作成
	for (const auto& registryItem : ItemFactory::GetInstance()->GetRegistryKinds())
	{
		m_ItemsSpawnCount[registryItem.first] = 0;
	}
}

SelectSpawnItem::~SelectSpawnItem()
{
}

ItemID SelectSpawnItem::SerectSpawnItem(std::vector<std::unique_ptr<ItemBase>>& items)
{
	//最初に現在の最小カウントを求める
	MinItemCount();

	//アイテムの出現数を確認して、少ないものを出すスポーンアイテムの選択
	for (auto& item : m_ItemsSpawnCount)
	{
		if (item.second == m_MinItemCount)
		{
			//アイテム生成カウント増加
			item.second += 1;
			return item.first;
		}
	}
	return ItemID::None;
}

void SelectSpawnItem::MinItemCount()
{
	//暫定最小値
	//とりあえずボムを入れて比較していく
	int preliminaryCount = m_ItemsSpawnCount[ItemID::Bomb];

	for (auto& item : m_ItemsSpawnCount)
	{
		//最小値を計算
		preliminaryCount = std::min(preliminaryCount, item.second);
	}

	//最小値を代入
	m_MinItemCount = preliminaryCount;
}
