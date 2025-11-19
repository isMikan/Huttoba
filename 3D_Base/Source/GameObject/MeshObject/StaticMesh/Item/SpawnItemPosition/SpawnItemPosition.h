#pragma once

#include "Ground/GroundManager/CGroundManager.h"


class SpawnItemPosition
{
public:

	SpawnItemPosition(CGroundManager& GroundMamager);
	~SpawnItemPosition();

	D3DXVECTOR3 SpawnItem();

private:

	//アイテムの出現地点
	std::vector<std::pair<GroundTag, D3DXVECTOR3>> m_pSpawnPosition;
	std::unordered_map<GroundTag, bool>			m_IsFallGrounds;

	CGroundManager& m_pGroundManager;
};