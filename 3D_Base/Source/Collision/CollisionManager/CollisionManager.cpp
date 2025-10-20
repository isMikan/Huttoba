#include "CollisionManager.h"
#include "Collision/Collider/CollisionBase.h"
#include "Collision/CollisionResult/CollisionResult.h"
#include "Collision/CollisionListener/ICollisionListener.h"
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
#include "Collision/CollisionStrategy/CollisionStrategyFactory/CollisionStrategyFactory.h"

#include <algorithm>

// Collider“o˜^
void CollisionManager::AddCollider(CollisionBase* col)
{
    if (!col) return;

    // d•¡“o˜^–h~
    auto it = std::find(m_Colliders.begin(), m_Colliders.end(), col);
    if (it == m_Colliders.end())
        m_Colliders.push_back(col);
}

// Collideríœ
void CollisionManager::RemoveCollider(CollisionBase* col)
{
    if (!col) return;

    auto it = std::find(m_Colliders.begin(), m_Colliders.end(), col);
    if (it != m_Colliders.end())
        m_Colliders.erase(it);
}

// Õ“Ë”»’èÀs
void CollisionManager::CheckCollisions()
{
    //ŒÄ‚Ño‚µ‚ª‘½‚¢‚Ì‚ÅƒLƒƒƒbƒVƒ…
    const size_t Colliders = m_Colliders.size();

    for (size_t i = 0; i < Colliders; ++i)
    {
        CollisionBase* colA = m_Colliders[i];
        if (!colA || !colA->GetActive()) continue;

        for (size_t j = i + 1; j < Colliders; ++j)
        {
            CollisionBase* colB = m_Colliders[j];
            if (!colB || !colB->GetActive()) continue;
            if (colA->GetOwner() == colB->GetOwner()) continue; 

            // Strategy‚ğæ“¾
            auto strategy = CollisionStrategyFactory::GetInstance()->GetStrategy(
                colA->GetType(), colB->GetType());
            if (!strategy) continue; 

            // Õ“Ë”»’è
            CollisionResult result = strategy->CheckCollision(colA, colB);
            if (!result.IsHit) continue;

            // Listener’Ê’m
            if (auto listenerA = colA->GetOwner())
                listenerA->OnCollision(colB,result);

            if (auto listenerB = colB->GetOwner())
                listenerB->OnCollision(colA,result);
        }
    }
}
