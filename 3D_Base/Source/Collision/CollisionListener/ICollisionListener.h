#pragma once
#include "Collision/CollisionResult/CollisionResult.h"

class ICollisionListener
{
public:
    virtual ~ICollisionListener() {}

    // Õ“Ë”»’èŒ‹‰Ê‚ğó‚¯æ‚éŠÖ”
    virtual void OnCollision(const CollisionResult& result) = 0;
};