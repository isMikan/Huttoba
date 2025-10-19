#include "CollisionManager.h"
#include "Collision/Collider/CollisionBase.h"
#include "Collision/CollisionResult/CollisionResult.h"
#include "Collision/CollisionListener/ICollisionListener.h"
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
#include "Collision/CollisionStrategy/CollisionStrategyFactory/CollisionStrategyFactory.h"

#include <algorithm>

// Colliderìoò^
void CollisionManager::AddCollider(CollisionBase* col)
{
    if (!col) return;

    // èdï°ìoò^ñhé~
    auto it = std::find(m_Colliders.begin(), m_Colliders.end(), col);
    if (it == m_Colliders.end())
        m_Colliders.push_back(col);
}

// ColliderçÌèú
void CollisionManager::RemoveCollider(CollisionBase* col)
{
    if (!col) return;

    auto it = std::find(m_Colliders.begin(), m_Colliders.end(), col);
    if (it != m_Colliders.end())
        m_Colliders.erase(it);
}

// è’ìÀîªíËé¿çs
void CollisionManager::CheckCollisions()
{
    for (size_t i = 0; i < m_Colliders.size(); ++i)
    {
        CollisionBase* colA = m_Colliders[i];
        if (!colA || !colA->GetActive()) continue;

        for (size_t j = i + 1; j < m_Colliders.size(); ++j)
        {
            CollisionBase* colB = m_Colliders[j];
            if (!colB || !colB->GetActive()) continue;

            // StrategyÇéÊìæ
            auto strategy = CollisionStrategyFactory::GetInstance()->GetStrategy(
                colA->GetType(), colB->GetType());

            if (!strategy) continue; // äYìñÇ∑ÇÈÉyÉAÇ™ñ¢ìoò^Ç»ÇÁÉXÉLÉbÉv

            // è’ìÀîªíË
            CollisionResult result = strategy->CheckCollision(colA, colB);
            if (!result.IsHit) continue;

            // Listenerí ím
            if (auto listenerA = colA->GetOwner())
                listenerA->OnCollision(result);

            if (auto listenerB = colB->GetOwner())
                listenerB->OnCollision(result);
        }
    }
}
