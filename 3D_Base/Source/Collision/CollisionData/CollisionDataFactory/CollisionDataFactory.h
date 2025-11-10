#pragma once

#include "Collision/Collider/CollisionSphere/CollisionSphere.h"
#include "Collision/Collider/CollisionCapsule/CollisionCapsule.h"
#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"
#include "GameObject/CGameObject.h"
#include "Collision/CollisionListener/ICollisionListener.h"
#include "Collision/CollisionUtility/CollisionUtility.h"


/*************************************
 * 衝突データを生成するファクトリクラス。
 *****/

class CollisionDataFactory
{
public:

    /*****************************************************************************************
    * @brief        CollisionSphereを生成し、親オブジェクトの情報と紐づける[
    * 
    * @param tag        識別タグ
    *        pMesh      当たり判定のメッシュ
    *        pOwner     このオブジェクトに追従する
    *        IsAddCollider 自動で判定登録するか(攻撃用判定などはここでは登録しない。Useになったときに登録しよう)
    * 
    * @return  所有権を持つ生成された CollisionSphere の shared_ptr
    *****************************************************************************************/
    static std::shared_ptr<CollisionBase> CreateSphereForMesh(
        CollisionBase::ColliderTag tag,
        std::shared_ptr<CStaticMesh> pMesh,
        CGameObject* pOwner,
        bool IsAddCollider = true);

    /*****************************************************************************************
    * @brief   CollisionCapsuleを生成し、親オブジェクトの情報と紐づける
    *
    * @param pOwner 衝突形状を持つ親オブジェクト
    * @param pMesh メッシュデータ（カプセルのサイズ計算に使用）
    * @param tag   コリジョンタグ
    *
    * @return  所有権を持つ生成された CollisionCapsule の shared_ptr
    *****************************************************************************************/
    static std::shared_ptr<CollisionBase> CreateCapsuleForMesh(
        CGameObject* pOwner,
        std::shared_ptr<CStaticMesh> pMesh,
        CollisionBase::ColliderTag tag
    );

    //横向きカプセル
    static std::shared_ptr<CollisionBase> CreateHorizontalCapsule(
        CollisionBase::ColliderTag tag,
        std::shared_ptr<CStaticMesh> pMesh,
        CGameObject* pOwner,
        bool IsAddCollider = true
    );

};