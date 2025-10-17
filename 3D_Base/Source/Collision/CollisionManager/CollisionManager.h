#pragma once
#include <vector>
#include <memory>
#include "Collision/Collider/CollisionBase.h"
#include "Collision/CollisionResult/CollisionResult.h"
#include "Collision/CollisionListener/ICollisionListener.h"
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
#include "Collision/CollisionStrategy/CollisionStrategyFactory/CollisionStrategyFactory.h"

class CollisionManager
{
public:

    static CollisionManager* GetInstance()
    {
        static CollisionManager instance;
        return &instance;
    }

    // Collider ‚Ì“o˜^/íœ
    void AddCollider(CollisionBase* col);
    void RemoveCollider(CollisionBase* col);

    // ”»’èÀs
    void CheckCollisions();

private:
    CollisionManager() {}
    ~CollisionManager() {}

    // “o˜^‚³‚ê‚Ä‚¢‚é Collider ƒŠƒXƒg
    std::vector<CollisionBase*> m_Colliders;

};
