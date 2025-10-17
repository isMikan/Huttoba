#include "stdafx.h"
#include "CollisionStrategyFactory.h"

void CollisionStrategyFactory::RegisterStrategy(const std::string& key, StrategyPtr strategy)
{
	m_Strategies[key] = std::move(strategy);
}

CollisionStrategyBase* CollisionStrategyFactory::GetStrategy(const std::string& key)
{
    auto it = m_Strategies.find(key);
    if (it != m_Strategies.end())
        return it->second.get();
    return nullptr;
}
