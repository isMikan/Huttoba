#pragma once
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
class CollisionCapsuleOBB
    : public CollisionStrategyBase
{
public:
    CollisionCapsuleOBB() {}
    ~CollisionCapsuleOBB() {}

    CollisionResult CheckCollision(
        const CollisionBase* colA,
        const CollisionBase* colB) const override;
};
