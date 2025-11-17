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

	//出現の抽選を行うアイテムを格納する変数
	std::vector<std::pair<ItemID, int>> possibilityItem;

	//アイテムの出現数を確認して、少ないものを出すスポーンアイテムの選択
	for (auto& item : m_ItemsSpawnCount)
	{
		if (item.second == m_MinItemCount)
		{
			//最小カウントと一致したアイテムを格納(抽選のため)
			possibilityItem.push_back(item);
		}
	}

	//ランダム設定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, possibilityItem.size() - 1);

	//ランダム抽選
	auto& elem = possibilityItem[dist(gen)];

	//抽選で選ばれたアイテムのIDを参照し、一致したものをカウントプラス
	for (auto& item : m_ItemsSpawnCount)
	{
		if (elem.first == item.first)
		{
			//アイテム生成カウント増加
			item.second += 1;

			return 	item.first;
		}
	}
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
