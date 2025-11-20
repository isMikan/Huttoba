#include "stdafx.h"
#include "SpawnItemPosition.h"
#include <algorithm>

SpawnItemPosition::SpawnItemPosition(std::unique_ptr<CGroundManager>& GroundMamager)
	: m_pSpawnPosition	{}
	, m_pGroundManager	{ GroundMamager }
	, m_IsFallGrounds	{}
{
	Init();
	for (auto& ground : m_IsFallGrounds)
	{
		ground.second = false;
	}
}

SpawnItemPosition::~SpawnItemPosition()
{
}

void SpawnItemPosition::Init()
{
	m_pSpawnPosition.clear();
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{-6,15,1} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{-2,15,2} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{ 5,15,3} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{ 9,15,4} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{-4,15,5} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{ 0,15,6} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{ 2,15,6} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{ 4,15,7} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{-6,15,8} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{-2,15,9} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{ 2,15,10} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{ 6,15,11} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{-4,15,12} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{-2,15,13} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{ 0,15,14} });
	m_pSpawnPosition.push_back({ GroundTag::FirstFallGround, D3DXVECTOR3{ 4,15,15} });
}

D3DXVECTOR3 SpawnItemPosition::SerectPosition()
{
	//ステージがすでに落ちているか判定
	for (auto& ground : m_pGroundManager->GetGrounds())
	{
		//ステージが落ちているか、落下情報はすでにtrueではないか
		if (ground->GetIsFallDown())
		{
			m_IsFallGrounds[ground->GetTag()] = true;
		}
	}

	//ランダム設定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, m_pSpawnPosition.size() - 1);

	int randomIndex = dist(gen);
	auto it = m_pSpawnPosition.begin();
	std::advance(it, randomIndex);

	std::pair<GroundTag, D3DXVECTOR3> selectPosition = *it;
	
	m_pSpawnPosition.erase(it);

	if (m_pSpawnPosition.size() <= 0)
	{
		Init();
	}

	return selectPosition.second;
}
