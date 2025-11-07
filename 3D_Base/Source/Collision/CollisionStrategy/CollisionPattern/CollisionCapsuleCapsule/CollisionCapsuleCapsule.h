#pragma once
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"

class CollisionBase;

class CollisionCapsuleCapsule 
    : public CollisionStrategyBase
{
public:
    CollisionCapsuleCapsule() {}
    ~CollisionCapsuleCapsule() {}

    bool CheckCollision(
        const CollisionBase* colA,
        const CollisionBase* colB) const override;
};
