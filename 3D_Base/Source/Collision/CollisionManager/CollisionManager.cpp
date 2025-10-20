#include "CollisionManager.h"
#include "Collision/Collider/CollisionBase.h"
#include "Collision/CollisionResult/CollisionResult.h"
#include "Collision/CollisionListener/ICollisionListener.h"
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"
#include "Collision/CollisionStrategy/CollisionStrategyFactory/CollisionStrategyFactory.h"

#include <algorithm>

// Collider登録
void CollisionManager::AddCollider(CollisionBase* col)
{
    if (!col) return;

    // 重複登録防止
    auto it = std::find(m_Colliders.begin(), m_Colliders.end(), col);
    if (it == m_Colliders.end())
        m_Colliders.push_back(col);
}

// Collider削除
void CollisionManager::RemoveCollider(CollisionBase* col)
{
    if (!col) return;

    auto it = std::find(m_Colliders.begin(), m_Colliders.end(), col);
    if (it != m_Colliders.end())
        m_Colliders.erase(it);
}

// 衝突判定実行
void CollisionManager::CheckCollisions()
{
    //呼び出しが多いのでキャッシュ
    const size_t Colliders = m_Colliders.size();

    for (size_t i = 0; i < Colliders; ++i)
    {
        //ColAを設定。
        CollisionBase* colA = m_Colliders[i];
        if (!colA || !colA->GetActive()) continue;

        for (size_t j = i + 1; j < Colliders; ++j)
        {
            //ColBを設定。
            CollisionBase* colB = m_Colliders[j];
            if (!colB || !colB->GetActive()) continue;
            if (colA->GetOwner() == colB->GetOwner()) continue;

            // Strategyを取得
            auto strategy = CollisionStrategyFactory::GetInstance()->GetStrategy(
                colA->GetType(), colB->GetType());
            if (!strategy) continue;

            // 衝突判定
            CollisionResult result = strategy->CheckCollision(colA, colB);
            if (!result.IsHit) continue;

            // コンソールにログ出力
            std::cout << "HIT " << std::endl;
            // Listener通知
            if (auto listenerA = colA->GetOwner())
            {
                listenerA->OnCollision(colB, result);
            }

            if (auto listenerB = colB->GetOwner())
                listenerB->OnCollision(colA, result);

            if (result.IsHit)
            {
                std::cout << "Hit" << std::endl;
            }
        }
    }
}
