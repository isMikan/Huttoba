#pragma once

#include "Collision/Collider/CollisionSphere/CollisionSphere.h"
#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"
#include "GameObject/CGameObject.h"
#include "Collision/CollisionListener/ICollisionListener.h"
#include "Collision/CollisionMeshUtility/MeshCollisionUtility.h"


/*************************************
 * 衝突データを生成するファクトリクラス。
 *****/

class CollisionDataFactory
{
public:


    /*****************************************************************************************
    * @brief        CollisionSphereを生成し、親オブジェクトの情報と紐づける[
    * 
    * @param pOwner 衝突形状を持つ親オブジェクト
    *        radius 球の半径
    *        tag    コリジョンタグ
    *        localOffset ローカルオフセット
    * 
    * @return  所有権を持つ生成された CollisionSphere の shared_ptr
    *****************************************************************************************/
    static std::shared_ptr<CollisionBase> CreateSphereForMesh(
        CGameObject* pOwner,
        std::shared_ptr<CStaticMesh> pMesh,
        CollisionBase::ColliderTag   tag
    );

    //@ToDo ここに判定追加


};