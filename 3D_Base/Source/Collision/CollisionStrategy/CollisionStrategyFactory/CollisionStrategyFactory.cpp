#include "CollisionStrategyFactory.h"
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
#include "Collision/Collider/CollisionBase.h"

void CollisionStrategyFactory::RegisterStrategy(CollisionBase::ColliderType typeA,
    CollisionBase::ColliderType typeB,
    StrategyPtr strategy)
{
    m_Strategies[{typeA, typeB}] = std::move(strategy);
}

CollisionStrategyBase* CollisionStrategyFactory::GetStrategy(CollisionBase::ColliderType typeA,
    CollisionBase::ColliderType typeB)
{
    auto it = m_Strategies.find({ typeA, typeB });
    if (it != m_Strategies.end())
        return it->second.get();
    return nullptr;
}
