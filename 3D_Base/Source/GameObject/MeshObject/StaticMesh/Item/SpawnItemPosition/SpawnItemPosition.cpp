#include "stdafx.h"
#include "SpawnItemPosition.h"

SpawnItemPosition::SpawnItemPosition(CGroundManager& GroundMamager)
	: m_pSpawnPosition	{}
	, m_pGroundManager	{ GroundMamager }
	, m_IsFallGrounds	{}
{
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{0,0,0} });
	m_pSpawnPosition.push_back({ GroundTag::SecondFallGround,D3DXVECTOR3{1,0,1} });
	m_pSpawnPosition.push_back({ GroundTag::ThirdFallGround, D3DXVECTOR3{2,0,2} });
	m_pSpawnPosition.push_back({ GroundTag::SafeGround,		 D3DXVECTOR3{3,0,3} });
	m_pSpawnPosition.push_back({ GroundTag::ThirdFallGround, D3DXVECTOR3{4,0,4} });

	for (auto& ground : m_IsFallGrounds)
	{
		ground.second = false;
	}
}

SpawnItemPosition::~SpawnItemPosition()
{
}

D3DXVECTOR3 SpawnItemPosition::SpawnItem()
{
	//ステージがすでに落ちているか判定
	for (auto& ground : m_pGroundManager.GetGrounds())
	{
		//ステージが落ちているか、落下情報はすでにtrueではないか
		if (ground->GetIsFallDown() || m_IsFallGrounds[ground->GetTag()] != true)
		{
			m_IsFallGrounds[ground->GetTag()] = true;
		}
	}

	//ランダム設定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, m_pSpawnPosition.size() - 1);

	//ランダム抽選
	std::pair<GroundTag, D3DXVECTOR3> selectPosition = m_pSpawnPosition[dist(gen)];

	return selectPosition.second;
}
