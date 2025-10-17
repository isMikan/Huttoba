#include "CollisionManager.h"
#include "Collision/Collider/CollisionBase.h"
#include "Collision/CollisionResult/CollisionResult.h"
#include "Collision/CollisionListener/ICollisionListener.h"
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
#include "Collision/CollisionStrategy/CollisionStrategyFactory/CollisionStrategyFactory.h"

#include <algorithm>


void CollisionManager::AddCollider(CollisionBase* col)
{
    if (col && std::find(m_Colliders.begin(), m_Colliders.end(), col) == m_Colliders.end())
        m_Colliders.push_back(col);
}

void CollisionManager::RemoveCollider(CollisionBase* col)
{
    auto it = std::find(m_Colliders.begin(), m_Colliders.end(), col);
    if (it != m_Colliders.end())
        m_Colliders.erase(it);
}

void CollisionManager::CheckCollisions()
{
    for (size_t i = 0; i < m_Colliders.size(); ++i)
    {
        CollisionBase* colA = m_Colliders[i];
        if (!colA->GetActive()) continue;

        for (size_t j = i + 1; j < m_Colliders.size(); ++j)
        {
            CollisionBase* colB = m_Colliders[j];
            if (!colB->GetActive()) continue;

            auto tagA = colA->GetTag();
            auto tagB = colB->GetTag();

            // ===== タグで衝突判定するかフィルタ =====
            bool shouldCheck = false;

            switch (tagA)
            {
            case CollisionBase::ColliderTag::Player:
                shouldCheck = (tagB == CollisionBase::ColliderTag::Item ||
                    tagB == CollisionBase::ColliderTag::Haetataki);
                break;

            case CollisionBase::ColliderTag::Ground:
                shouldCheck = (tagB == CollisionBase::ColliderTag::Player ||
                    tagB == CollisionBase::ColliderTag::Item);
                break;

                // 必要に応じて追加
            default:
                break;
            }

            if (!shouldCheck) continue;

            // ===== Strategyを選択 =====
            auto strategy = CollisionStrategyFactory::GetInstance()->GetStrategy(colA->GetType(), colB->GetType());
            if (!strategy) continue;

            CollisionResult result = strategy->CheckCollision(colA, colB);

            if (result.IsHit)
            {
                colA->GetOwner()->OnCollision(result);
                colB->GetOwner()->OnCollision(result);
            }
        }
    }
}