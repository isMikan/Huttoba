#include "stdafx.h"
#include "CollisionDataFactory.h"

std::shared_ptr<CollisionBase> CollisionDataFactory::CreateSphereForMesh(
    CGameObject* pOwner,
    std::shared_ptr<CStaticMesh> pMesh,
    CollisionBase::ColliderTag tag)
{
    D3DXVECTOR3 calculatedCenter(0.0f, 0.0f, 0.0f);
    float calculatedRadius = 0.0f;

    // ユーティリティ関数を呼び出して中心と半径を計算
    if (!MeshCollisionUtility::CalculateBoundingSphere(
        pMesh, calculatedCenter, calculatedRadius))
    {
        // 計算失敗時はnullptrを返すか、エラー処理を行う
        return nullptr;
    }

    ICollisionListener* listener = dynamic_cast<ICollisionListener*>(pOwner);
    const D3DXVECTOR3& posRef = pOwner->GetPosition();

    // CStaticMeshObjectの中心座標は通常(0,0,0)だが、モデル原点がオフセットされている場合は
    // calculatedCenterをlocalOffsetとして渡す

    // CollisionSphereのインスタンスを生成 (計算結果を利用)
    std::shared_ptr<CollisionBase> newCollider = std::make_shared<CollisionSphere>(
        listener,
        posRef,
        tag,
        calculatedRadius,      // 計算された半径
        calculatedCenter       // 計算されたオフセット
    );

    CollisionManager::GetInstance()->AddCollider(newCollider);

    return newCollider;
}

std::shared_ptr<CollisionBase> CollisionDataFactory::CreateCapsuleForMesh(
    CGameObject* pOwner, 
    std::shared_ptr<CStaticMesh> pMesh,
    CollisionBase::ColliderTag tag)
{
    // カプセルに必要なローカル情報
    float calculatedRadius = 0.0f;
    D3DXVECTOR3 localOffsetA(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 localOffsetB(0.0f, 0.0f, 0.0f);

    // 1. メッシュからカプセルのパラメータを計算
    if (!MeshCollisionUtility::CalculateBoundingCapsule(
        pMesh, calculatedRadius, localOffsetA, localOffsetB))
    {
        // 計算失敗時はnullptrを返すか、エラー処理を行う
        return nullptr;
    }

    ICollisionListener* listener = dynamic_cast<ICollisionListener*>(pOwner);
    const D3DXVECTOR3& posRef = pOwner->GetPosition();

    // 2. CollisionCapsuleのインスタンスを生成
    std::shared_ptr<CollisionBase> newCollider = std::make_shared<CollisionCapsule>(
        listener,
        posRef,               // SyncPositionとして親の位置を参照
        tag,
        calculatedRadius,
        localOffsetA,         // 軸線分Aのローカルオフセット
        localOffsetB          // 軸線分Bのローカルオフセット
        // CollisionBase::localOffsetBase はデフォルト引数で省略
    );

    // 3. 【★重要】生成と同時にマネージャーに登録
    CollisionManager::GetInstance()->AddCollider(newCollider);

    return newCollider;
}
