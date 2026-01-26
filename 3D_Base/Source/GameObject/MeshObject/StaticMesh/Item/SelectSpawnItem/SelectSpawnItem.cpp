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

//=======================================================
//				抽選管理部分
//=======================================================

ItemID SelectSpawnItem::SerectSpawnItem(std::vector<std::unique_ptr<ItemBase>>& items)
{
	//最初に現在の最小カウントを求める
	MinItemCount();

	//全種類のアイテムカウントと、最小カウントを比較し、抽選に参加するアイテムを選定
	auto possibilityItem = LotterySelect();

	//選定したアイテムでランダム抽選し、結果を格納
	auto elem = Lottery(possibilityItem);

	//抽選で選ばれたアイテムのIDを比較し、一致したものをカウントプラス + 返り値で返す
	for (auto& item : m_ItemsSpawnCount)
	{
		//ID比較
		if (elem.first == item.first)
		{
			//アイテム生成カウント増加
			item.second += 1;

			//選択されたアイテムを返す
			return 	item.first;
		}
	}
	return ItemID::None;
}

//以下機能
//--------------------------------------------------------------------------------------------------------------

void SelectSpawnItem::MinItemCount()
{
	//暫定最小値
	//とりあえずボムを入れて比較していく
	int minCount = m_ItemsSpawnCount[ItemID::Bomb];

	for (auto& item : m_ItemsSpawnCount)
	{
		//最小値を計算
		minCount = std::min(minCount, item.second);
	}

	 m_MinItemCount = minCount;
}

//--------------------------------------------------------------------------------------------------------------

std::vector<std::pair<ItemID, int>> SelectSpawnItem::LotterySelect()
{
	//抽選に参加するアイテムを格納する配列
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
	return possibilityItem;
}

//--------------------------------------------------------------------------------------------------------------

std::pair<ItemID, int> SelectSpawnItem::Lottery(std::vector<std::pair<ItemID, int>> LotterySelectItems)
{
	//ランダム設定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, LotterySelectItems.size() - 1);

	//ランダム抽選
	std::pair<ItemID, int> selectItem = LotterySelectItems[dist(gen)];

	return selectItem;
}