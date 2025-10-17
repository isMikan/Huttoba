#pragma once
#include <unordered_map>
#include <memory>
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
#include "Collision/Collider/CollisionBase.h"

class CollisionStrategyFactory
{
public:
    using StrategyPtr = std::unique_ptr<CollisionStrategyBase>;

    // ƒVƒ“ƒOƒ‹ƒgƒ“‚É‚·‚éê‡
    static CollisionStrategyFactory& GetInstance()
    {
        static CollisionStrategyFactory instance;
        return instance;
    }

    // Strategy “o˜^
    void RegisterStrategy(const std::string& key, StrategyPtr strategy);

    // Strategy æ“¾
    CollisionStrategyBase* GetStrategy(const std::string& key);

private:
    CollisionStrategyFactory() {}
    std::unordered_map<std::string, StrategyPtr> m_Strategies;
};