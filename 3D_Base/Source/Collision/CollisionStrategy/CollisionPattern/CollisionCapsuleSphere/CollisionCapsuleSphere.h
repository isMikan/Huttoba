#pragma once
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
class CollisionCapsuleSphere :
    public CollisionStrategyBase
{
public:
    CollisionCapsuleSphere() {}
    ~CollisionCapsuleSphere() {}

    CollisionResult CheckCollision(
        const CollisionBase* colA,
        const CollisionBase* colB) const override;
};

