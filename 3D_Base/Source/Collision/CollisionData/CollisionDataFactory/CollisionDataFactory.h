#pragma once

#include "Collision/Collider/CollisionSphere/CollisionSphere.h"
#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

class CollisionDataFactory
{
public:
    /**
     * @brief メッシュから球体のデータを抽出し、CollisionSphereを初期化する。
     * * @param pMesh       入力となるメッシュ
     * @param outSphere   初期化されるCollisionSphereオブジェクト
     * @return HRESULT
     */
    static HRESULT CreateSphereDataFromMesh(
        const std::shared_ptr<CStaticMesh> pMesh,
        CollisionSphere* outSphere);
};