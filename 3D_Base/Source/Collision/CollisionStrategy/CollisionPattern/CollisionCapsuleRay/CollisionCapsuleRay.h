#pragma once
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
class CollisionCapsuleRay :
    public CollisionStrategyBase
{
public:
    CollisionCapsuleRay() {}
    ~CollisionCapsuleRay() {}

    bool CheckCollision(
        const CollisionBase* colA,
        const CollisionBase* colB) const = 0;
};

