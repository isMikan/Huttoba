#pragma once

#include "Collision/Collider/CollisionSphere/CollisionSphere.h"
#include "Collision/Collider/CollisionCapsule/CollisionCapsule.h"
#include "CStaticMeshObject.h"
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
        CollisionBase::ColliderTag tag,
        std::shared_ptr<CStaticMesh> pMesh,
        CGameObject* pOwner,
        bool IsAddCollider = true
    );

    
    /*****************************************************************************************
    * @brief   CollisionCapsuleを生成し、親オブジェクトの情報と紐づける
    *
    * @param pOwner 衝突形状を持つ親オブジェクト
    * @param pMesh メッシュデータ（カプセルのサイズ計算に使用）
    * @param tag   コリジョンタグ
    *
    * @return  所有権を持つ生成された CollisionCapsule の shared_ptr
    *****************************************************************************************/
    static std::shared_ptr<CollisionBase> CreateHorizontalCapsuleForMesh(
        CollisionBase::ColliderTag tag,
        std::shared_ptr<CStaticMesh> pMesh,
        CGameObject* pOwner,
        bool IsAddCollider = true
    );

};