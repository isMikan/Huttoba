#include "stdafx.h"
#include "CollisionManager.h"
#include <algorithm>
#include <iostream> // デバッグ用

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

// 衝突判定ロジック (Strategyパターンの簡易シミュレーション)
bool CollisionManager::CheckCollision(CollisionBase* a, CollisionBase* b)
{
    // 不要な判定をスキップ
    if (a->GetTag() == CollisionBase::ColliderTag::Ground && b->GetTag() == CollisionBase::ColliderTag::Ground)
    {
        return false;
    }

    // 2. 【Strategy切替】形状タイプに応じて判定関数を呼び出す
    //    これはタグだけでなく、タイプ(Sphere, OBBなど)の組み合わせによって決定されます。
    //    例: SphereとSphereの判定
    if (a->GetType() == CollisionBase::ColliderType::Sphere && b->GetType() == CollisionBase::ColliderType::Sphere)
    {
        // ここで具体的なSphere-Sphere判定ロジックを呼び出す
        CollisionSphere* sphereA = static_cast<CollisionSphere*>(a);
        CollisionSphere* sphereB = static_cast<CollisionSphere*>(b);

        D3DXVECTOR3 diff = sphereA->GetWorldPosition() - sphereB->GetWorldPosition();
        float distSq = D3DXVec3LengthSq(&diff);
        float radiusSum = sphereA->GetRadius() + sphereB->GetRadius();

        return distSq <= (radiusSum * radiusSum);
    }

    // 他の組み合わせ (Sphere-Box, Box-Capsuleなど) のStrategyがここに追加される...

    return false; // 未対応の組み合わせは衝突なしとする
}

void CollisionManager::Update(float deltaTime)
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