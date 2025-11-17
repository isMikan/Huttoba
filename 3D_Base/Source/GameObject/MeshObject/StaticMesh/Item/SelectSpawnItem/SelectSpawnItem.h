#pragma once

class ItemBase;

class SelectSpawnItem
{
public:
	SelectSpawnItem();
	~SelectSpawnItem();

	ItemID SerectSpawnItem(std::vector<std::unique_ptr<ItemBase>>& items);

private:

	void MinItemCount();

private:

	std::unordered_map<ItemID, int> m_ItemsSpawnCount;
	bool	m_IsSpawnMagnet;
	int		m_MinItemCount;

};