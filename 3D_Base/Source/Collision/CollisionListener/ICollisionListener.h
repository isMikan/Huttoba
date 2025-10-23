#pragma once

class CollisionBase;

class ICollisionListener
{
public:
    virtual ~ICollisionListener() {}

    // Õ“Ë”»’èŒ‹‰Ê‚ğó‚¯æ‚éŠÖ”
    virtual void OnCollision(CollisionBase* other) = 0;
};