#include "stdafx.h"
#include "CollisionManager.h"
#include <algorithm>
#include "Collision/CollisionStrategy/CollisionStrategyFactory/CollisionStrategyFactory.h"
#include "Collision/CollisionStrategy/CollisionStrategyBase.h"

#include "Collision/CollisionStrategy/CollisionPattern/CollisionSphereSphere/CollisionStrategySphereSphere.h"
#include "Collision/CollisionStrategy/CollisionPattern/CollisionSphereCapsule/CollisionSphereCapsule.h"
#include "Collision/CollisionStrategy/CollisionPattern/CollisionCapsuleCapsule/CollisionCapsuleCapsule.h"

#include "Ground/GroundManager/CGroundManager.h"

CollisionManager::CollisionManager()
	: m_Colliders   {}
{

}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Init()
{
	m_Colliders.clear();
}

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
    //  StrategyFactoryから判定ストラテジーを取得
    CollisionStrategyBase* strategy = CollisionStrategyFactory::GetInstance()->GetStrategy(
        a->GetType(),
        b->GetType()
    );

    // 判定ロジックが存在しない（未対応の組み合わせ）場合はスキップ
    if (strategy == nullptr){return false;}

    // 判定ロジックが存在する場合は、StrategyオブジェクトのCheckCollisionを呼び出す
    return strategy->CheckCollision(a, b);

}

// 許容誤差は判定ロジック（Raycastの最大距離）の中で定数として保持
// これなら外部から渡す必要がなくなり、シンプルになります。
static constexpr float GROUND_CHECK_EPSILON = 5.f;

bool CollisionManager::CheckGroundContact(
    const D3DXVECTOR3& objectPosition,
    float colliderHalfHeight,
    CGroundManager* pGroundMgr)
{
    if (!pGroundMgr) return false;

    // レイ設定 
    // @note ステージがずれているので D3DXVECTOR3{ 0.f, 0.f, -10.f }修正
    D3DXVECTOR3 rayOrigin = (objectPosition + D3DXVECTOR3{ 0.f, 0.f, -10.f }) - D3DXVECTOR3(0.0f, colliderHalfHeight - 0.01f, 0.0f);
    D3DXVECTOR3 rayDirection(0.0f, -1.0f, 0.0f);
    const float maxDistance = GROUND_CHECK_EPSILON + 0.01f;
    // Raycastのout引数
    D3DXVECTOR3 hitPosition;
    float currentHitDistance;

    // GroundManagerから配列を取得
    const auto& grounds = pGroundMgr->GetGrounds();

    // 地面の数だけ回す
    for (const auto& pGround : grounds)
    {
        if (!pGround) continue;

        // メッシュを取得
        std::shared_ptr<CStaticMesh> pGroundMesh = pGround->GetMesh();
        if (!pGroundMesh) continue;

        // MeshCollisionUtilityに判定を委譲
        if (MeshCollisionUtility::RaycastAgainstMesh(
            pGroundMesh,                 // ループ内で取得したメッシュ
            rayOrigin,
            rayDirection,
            maxDistance,
            hitPosition,                 // out
            currentHitDistance           // out
        ))
        {
            // 一つでも当たれば終了
            return true;
        }
    }

    return false;
}

// =========================================================================
// 地面接触判定 (カプセル vs メッシュの簡略化版)
// =========================================================================
void CollisionManager::RegisterStrategy()
{
    //当たり判定戦略の設定

    //球vs球----------------------------------------------------------------

    CollisionStrategyFactory::GetInstance()->RegisterStrategy(
        CollisionBase::ColliderType::Sphere,
        CollisionBase::ColliderType::Sphere,
        std::make_unique<CollisionStrategySphereSphere>()
    );

    //球vsカプセル-----------------------------------------------------------
    CollisionStrategyFactory::GetInstance()->RegisterStrategy(
        CollisionBase::ColliderType::Sphere,
        CollisionBase::ColliderType::Capsule,
        std::make_unique<CollisionSphereCapsule>()
    );

    //カプセルvsカプセル-----------------------------------------------------
    CollisionStrategyFactory::GetInstance()->RegisterStrategy(
        CollisionBase::ColliderType::Capsule,
        CollisionBase::ColliderType::Capsule,
        std::make_unique<CollisionCapsuleCapsule>()
    );

    //-----------------------------------------------------------------------

}

void CollisionManager::Update()
{
    //判定の位置同期
    for (const auto& pCollider : m_Colliders)
    {
        if (pCollider->GetActive())
        {
            // 親GameObjectの現在位置に追従
            pCollider->UpdateWorldPosition(); 
            // ヒットフラグをリセット(一旦)
            pCollider->SetHit(false);
        }
    }

    // 衝突判定
    for (size_t i = 0; i < m_Colliders.size(); ++i)
    {
        //この判定はアクティブか？
        CollisionBase* colliderA = m_Colliders[i].get();
        if (!colliderA->GetActive()) continue;

        for (size_t j = i + 1; j < m_Colliders.size(); ++j)
        {
            //この判定はアクティブか？
            CollisionBase* colliderB = m_Colliders[j].get();
            if (!colliderB->GetActive()) continue;

            // 判定実行と応答
            if (CheckCollision(colliderA, colliderB))
            {
                // コリジョン応答 (リスナーを呼び出す)
                colliderA->InvokeCollision(colliderB);
                colliderB->InvokeCollision(colliderA);

                // ヒットフラグを設定
                colliderA->SetHit(true);
                colliderB->SetHit(true);

            }
        }
    }
}