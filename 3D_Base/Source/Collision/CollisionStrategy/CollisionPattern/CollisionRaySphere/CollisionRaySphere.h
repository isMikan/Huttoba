#pragma once
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
class CollisionRaySphere :
    public CollisionStrategyBase
{
public:
    CollisionRaySphere() {}
    ~CollisionRaySphere() {}

    CollisionResult CheckCollision(
        const CollisionBase* colA,
        const CollisionBase* colB) const override;
};

