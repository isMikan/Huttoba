#include "stdafx.h"
#include "CollisionManager.h"
#include <algorithm>
#include <iostream> // デバッグ用
#include "Collision/CollisionStrategy/CollisionStrategyFactory/CollisionStrategyFactory.h"
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"

void CollisionManager::AddCollider(std::shared_ptr<CollisionBase> pCollider)
{
    if (pCollider)
    {
        m_Colliders.push_back(pCollider);
    }
}

void CollisionManager::RemoveCollider(CollisionBase* pColliderToRemove)
{
    if (!pColliderToRemove) return;

    // 生ポインタが指す要素をリストからスマートポインタの比較で検索し、削除する
    m_Colliders.erase(
        std::remove_if(m_Colliders.begin(), m_Colliders.end(),
            [pColliderToRemove](const std::shared_ptr<CollisionBase>& pCollider) {
                return pCollider.get() == pColliderToRemove;
            }),
        m_Colliders.end());
}

// 衝突判定ロジック
bool CollisionManager::CheckCollision(CollisionBase* a, CollisionBase* b)
{
    // 1. StrategyFactoryから判定ストラテジーを取得
    CollisionStrategyBase* strategy = CollisionStrategyFactory::GetInstance()->GetStrategy(
        a->GetType(),
        b->GetType()
    );

    // 2. 判定ロジックが存在しない（未対応の組み合わせ）場合はスキップ
    if (strategy == nullptr){return false;}

    // 3. 判定ロジックが存在する場合は、StrategyオブジェクトのCheckCollisionを呼び出す
    return strategy->CheckCollision(a, b);

}

void CollisionManager::Update()
{
    // 1. 【位置同期】 全てのコリジョンデータを更新
    for (const auto& pCollider : m_Colliders)
    {
        if (pCollider->GetActive())
        {
            pCollider->UpdateWorldPosition(); // 親GameObjectの現在位置に追従
            // ヒットフラグをリセット (次のフレームのために)
            pCollider->SetHit(false);
        }
    }

    // 2. 【衝突判定】 全ての有効なペアをチェック
    for (size_t i = 0; i < m_Colliders.size(); ++i)
    {
        CollisionBase* colliderA = m_Colliders[i].get();

        if (!colliderA->GetActive()) continue; // 非アクティブなものはスキップ

        for (size_t j = i + 1; j < m_Colliders.size(); ++j)
        {
            CollisionBase* colliderB = m_Colliders[j].get();

            if (!colliderB->GetActive()) continue; // 非アクティブなものはスキップ

            // 3. 判定実行と応答
            if (CheckCollision(colliderA, colliderB))
            {
                // 衝突があった場合

                // コリジョン応答 (リスナーを呼び出す)
                colliderA->InvokeCollision(colliderB);
                colliderB->InvokeCollision(colliderA);

                // ヒットフラグを設定
                colliderA->SetHit(true);
                colliderB->SetHit(true);

                // デバッグ出力
                // std::cout << "Collision detected between Tag " << (int)colliderA->GetTag() << " and " << (int)colliderB->GetTag() << std::endl;
            }
        }
    }

    // 4. (必要に応じて) 削除待ちのオブジェクトをクリーンアップ
    // ...
}