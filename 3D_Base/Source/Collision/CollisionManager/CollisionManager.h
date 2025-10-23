#pragma once

#include "Collision/Collider/CollisionBase.h" // CollisionBase, ColliderTag, ColliderType が必要
#include "Collision/Collider/CollisionSphere/CollisionSphere.h" // 判定ロジック内で具体的に利用される可能性を考慮

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

    // コリジョンデータをリストに追加し、所有権を受け取る
    void AddCollider(std::shared_ptr<CollisionBase> pCollider);
    // 衝突判定から除外/破棄されたオブジェクトの削除処理
    void RemoveCollider(CollisionBase* pColliderToRemove);

    // メインループから毎フレーム呼び出される更新処理
    void Update(float deltaTime);

    // 衝突判定ロジックを実行する関数 (Strategyの切り替えを含む)
    bool CheckCollision(CollisionBase* a, CollisionBase* b);

private:
    CollisionManager() = default;
    ~CollisionManager() = default;

    CollisionManager(const CollisionManager&) = delete; // コピーコンストラクタを削除
    CollisionManager& operator=(const CollisionManager&) = delete; // 代入演算子を削除

    // 登録されているColliderのリスト
    std::vector<std::shared_ptr<CollisionBase>> m_Colliders;

};
