#pragma once
#include <unordered_map>
#include <memory>
#include "../../Collider/CollisionBase.h"

class CollisionStrategyBase;

class CollisionStrategyFactory
{
public:
    using StrategyPtr = std::unique_ptr<CollisionStrategyBase>;

    static CollisionStrategyFactory* GetInstance()
    {
        static CollisionStrategyFactory instance;
        return &instance;
    }

    void RegisterStrategy(CollisionBase::ColliderType typeA,
        CollisionBase::ColliderType typeB,
        StrategyPtr strategy);

    CollisionStrategyBase* GetStrategy(CollisionBase::ColliderType typeA,
        CollisionBase::ColliderType typeB);

private:
    CollisionStrategyFactory() = default;
    ~CollisionStrategyFactory() = default;

    struct PairHash
    {
        size_t operator()(const std::pair<CollisionBase::ColliderType, CollisionBase::ColliderType>& p) const noexcept
        {
            size_t h1 = std::hash<int>()(static_cast<int>(p.first));
            size_t h2 = std::hash<int>()(static_cast<int>(p.second));

            // ï¿Ç—ë÷Ç¶ÇƒèÌÇ…è¨Ç≥Ç¢ï˚ÇêÊÇ…
            if (h1 > h2) std::swap(h1, h2);

            // BoostéÆçáê¨
            return h1 ^ (h2 << 1);
        }
    };

    std::unordered_map<std::pair<CollisionBase::ColliderType, CollisionBase::ColliderType>,
        StrategyPtr, PairHash> m_Strategies;
};