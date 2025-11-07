#pragma once
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"

class CollisionBase;

class CollisionSphereCapsule
    : public CollisionStrategyBase
{
public:
    CollisionSphereCapsule() {}
    ~CollisionSphereCapsule() {}

    bool CheckCollision(
        const CollisionBase* colA,
        const CollisionBase* colB) const override;
};
