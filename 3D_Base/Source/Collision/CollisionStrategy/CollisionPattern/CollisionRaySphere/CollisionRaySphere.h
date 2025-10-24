#pragma once
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
class CollisionRaySphere :
    public CollisionStrategyBase
{
public:
    CollisionRaySphere() {}
    ~CollisionRaySphere() {}

    bool CheckCollision(
        const CollisionBase* colA,
        const CollisionBase* colB) const = 0;
};

