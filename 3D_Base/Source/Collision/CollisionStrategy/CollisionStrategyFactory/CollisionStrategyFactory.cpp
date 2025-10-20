#include "CollisionStrategyFactory.h"
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
#include "Collision/Collider/CollisionBase.h"

void CollisionStrategyFactory::RegisterStrategy(CollisionBase::ColliderType typeA,
    CollisionBase::ColliderType typeB,
    StrategyPtr strategy)
{
    // í‚É¬‚³‚¢‡‚ÅŠi”[
    if (typeA > typeB) std::swap(typeA, typeB); 
    m_Strategies[{typeA, typeB}] = std::move(strategy);
}

CollisionStrategyBase* CollisionStrategyFactory::GetStrategy(
    CollisionBase::ColliderType typeA,
    CollisionBase::ColliderType typeB)
{
    // ”O‚Ì‚½‚ß‚à‚¤ˆê“x‡”Ô“ü‚ê‘Ö‚¦
    if (typeA > typeB) std::swap(typeA, typeB); 
    auto it = m_Strategies.find({ typeA, typeB });
    if (it != m_Strategies.end())
        return it->second.get();

    return nullptr;
}
