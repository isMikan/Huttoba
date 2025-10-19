#pragma once
#include <vector>
#include <memory>

class CollisionBase;
class CollisionResult;
class ICollisionListener;
class CollisionStrategyBase;

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

    // “o˜^‚³‚ê‚Ä‚¢‚éCollider‚ÌƒŠƒXƒg
    std::vector<CollisionBase*> m_Colliders;

};
