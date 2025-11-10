#include "stdafx.h"
#include "CollisionDataFactory.h"
#include "Collision/CollisionDraw/CollisionDraw.h"

std::shared_ptr<CollisionBase> CollisionDataFactory::CreateSphereForMesh(
    CollisionBase::ColliderTag tag,
    std::shared_ptr<CStaticMesh> pMesh,
    CGameObject* pOwner,
    bool IsAddCollider)
{
    D3DXVECTOR3 calculatedCenter(0.0f, 0.0f, 0.0f);
    float calculatedRadius = 0.0f;

    // ユーティリティ関数を呼び出して中心と半径を計算
    if (!CollisionUtility::CalculateBoundingSphere(
        pMesh, calculatedCenter, calculatedRadius))
    {
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

    if (IsAddCollider)
    {
        CollisionManager::GetInstance()->AddCollider(newCollider);
        CollisionDraw::GetInstance()->AddDrawMesh(pMesh, pOwner);
    }

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

    // メッシュからカプセルのパラメータを計算
    if (!CollisionUtility::CalculateBoundingCapsule(
        pMesh, calculatedRadius, localOffsetA, localOffsetB))
    {
        return nullptr;
    }

    ICollisionListener* listener = dynamic_cast<ICollisionListener*>(pOwner);
    const D3DXVECTOR3& posRef = pOwner->GetPosition();

    // CollisionCapsuleのインスタンスを生成
    std::shared_ptr<CollisionBase> newCollider = std::make_shared<CollisionCapsule>(
        listener,
        posRef,               // SyncPositionとして親の位置を参照
        tag,
        calculatedRadius,
        localOffsetA,         // 軸線分Aのローカルオフセット
        localOffsetB          // 軸線分Bのローカルオフセット
    );

    CollisionManager::GetInstance()->AddCollider(newCollider);
    CollisionDraw::GetInstance()->AddDrawMesh(pMesh, pOwner);

    return newCollider;
}

std::shared_ptr<CollisionBase> CollisionDataFactory::CreateHorizontalCapsule(
    CollisionBase::ColliderTag tag,
    std::shared_ptr<CStaticMesh> pMesh,
    CGameObject* pOwner,
    bool IsAddCollider
)
{
    // カプセルに必要なローカル情報
    float calculatedRadius = 0.0f;
    D3DXVECTOR3 localOffsetA(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 localOffsetB(0.0f, 0.0f, 0.0f);

    // メッシュからカプセルのパラメータを計算
    if (!CollisionUtility::CalculateHorizontalCapsule(
        pMesh, calculatedRadius, localOffsetA, localOffsetB))
    {
        return nullptr;
    }

    ICollisionListener* listener = dynamic_cast<ICollisionListener*>(pOwner);
    const D3DXVECTOR3& posRef = pOwner->GetPosition();

    // CollisionCapsuleのインスタンスを生成
    std::shared_ptr<CollisionBase> newCollider = std::make_shared<CollisionCapsule>(
        listener,
        posRef,               // SyncPositionとして親の位置を参照
        tag,
        calculatedRadius,
        localOffsetA,         // 軸線分Aのローカルオフセット
        localOffsetB          // 軸線分Bのローカルオフセット
    );

    if (IsAddCollider)
    {
        CollisionManager::GetInstance()->AddCollider(newCollider);
        CollisionDraw::GetInstance()->AddDrawMesh(pMesh, pOwner);
    }

    return newCollider;
}
