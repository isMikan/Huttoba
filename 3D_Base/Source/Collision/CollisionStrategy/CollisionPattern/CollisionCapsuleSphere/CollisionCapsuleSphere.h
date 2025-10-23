#pragma once
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
class CollisionCapsuleSphere :
    public CollisionStrategyBase
{
public:
    CollisionCapsuleSphere() {}
    ~CollisionCapsuleSphere() {}

    bool CheckCollision(
        const CollisionBase* colA,
        const CollisionBase* colB) const = 0;
};

