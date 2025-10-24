#pragma once

#include "Collision/Collider/CollisionBase.h"
#include "Collision/Collider/CollisionSphere/CollisionSphere.h" 

class CollisionBase;
class CollisionResult;
class ICollisionListener;
class CollisionStrategyBase;

class CollisionManager
{
public:

    /*****************************************************************************************
    * @brief   このクラスのポインタを返す
    *****************************************************************************************/
    static CollisionManager* GetInstance()
    {
        static CollisionManager instance;
        return &instance;
    }


    /*****************************************************************************************
    * @brief    コリジョンデータをリストに追加し、所有権を受け取る
    * @param    登録したい当たり判定
    *****************************************************************************************/
    void AddCollider(std::shared_ptr<CollisionBase> pCollider);


    /*****************************************************************************************
    * @brief    衝突判定から除外/破棄された当たり判定の削除処理
    * @param    破棄したい当たり判定
    *****************************************************************************************/
    void RemoveCollider(CollisionBase* pColliderToRemove);


    /*****************************************************************************************
    * @brief    メインループから毎フレーム呼び出される更新処理
    *****************************************************************************************/
    void Update();


    /*****************************************************************************************
    * @brief    衝突判定を実行する関数 (Strategyの切り替えを含む)
    * @param    a : 1つ目の判定
    * @param    b : 2つ目の判定
    *****************************************************************************************/
    bool CheckCollision(CollisionBase* a, CollisionBase* b);

private:
    CollisionManager() = default;
    ~CollisionManager() = default;

    CollisionManager(const CollisionManager&) = delete; // コピーコンストラクタを削除
    CollisionManager& operator=(const CollisionManager&) = delete; // 代入演算子を削除

    // 登録されているColliderのリスト
    std::vector<std::shared_ptr<CollisionBase>> m_Colliders;

};
