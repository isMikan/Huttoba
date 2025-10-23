#pragma once
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
class CollisionCapsuleOBB
    : public CollisionStrategyBase
{
public:
    CollisionCapsuleOBB() {}
    ~CollisionCapsuleOBB() {}

    bool CheckCollision(
        const CollisionBase* colA,
        const CollisionBase* colB) const = 0;
};
