#pragma once

#include "Ground/GroundManager/CGroundManager.h"


class SpawnItemPosition
{
public:

	SpawnItemPosition(std::unique_ptr<CGroundManager>& GManager);
	~SpawnItemPosition();

	D3DXVECTOR3 SerectPosition();

	void Init();

private:



	//アイテムの出現地点
	std::vector<std::pair<GroundTag, D3DXVECTOR3>> m_pSpawnPosition;
	std::unordered_map<GroundTag, bool>			m_IsFallGrounds;

	std::unique_ptr<CGroundManager>& m_pGroundManager;
};