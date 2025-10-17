#pragma once
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
class CollisionCapsuleCapsule 
    : public CollisionStrategyBase
{
public:
    CollisionCapsuleCapsule() {}
    ~CollisionCapsuleCapsule() {}

    CollisionResult CheckCollision(
        const CollisionBase* colA,
        const CollisionBase* colB) const override;
};
