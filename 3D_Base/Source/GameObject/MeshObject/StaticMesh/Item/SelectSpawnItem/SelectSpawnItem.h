#pragma once

class ItemBase;

static constexpr int SPAWN_LIMIT = 8;	//ステージ上のアイテム最大出現数

class SelectSpawnItem
{
public:
	SelectSpawnItem();
	~SelectSpawnItem();

	void SerectSpawnItem(std::vector<std::unique_ptr<ItemBase>> items);
	

private:

	int m_SpawnCount;
	bool IsSpawnMagnet;

};