#pragma once

class ItemBase;

class SelectSpawnItem
{
public:
	SelectSpawnItem(std::unordered_map<ItemID, bool>& SpawnItemData);
	~SelectSpawnItem();

	ItemID SerectSpawnItem(std::vector<std::unique_ptr<ItemBase>>& items);

private:

	//最小カウントを求める
	void MinItemCount();

	//出現アイテムに選択されているか
	bool IsOkSpawn(ItemID item);

	//抽選に入れるかどうか選択
	std::vector<std::pair<ItemID, int>> LotterySelect();

	//配列の中身を抽選して返す
	std::pair<ItemID, int> Lottery(std::vector<std::pair<ItemID, int>> LotterySelectItems);



private:

	std::unordered_map<ItemID, int> m_ItemsSpawnCount;
	std::unordered_map<ItemID, bool>& m_SpawnItemData;
	bool	m_IsSpawnMagnet;
	int		m_MinItemCount;

};