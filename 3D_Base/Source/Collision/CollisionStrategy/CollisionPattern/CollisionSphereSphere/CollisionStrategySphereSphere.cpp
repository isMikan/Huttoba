#include "CollisionStrategySphereSphere.h"
#include "GameObject/CGameObject.h"
#include "Collision/Collider/CollisionSphere/CollisionSphere.h"

// 【修正】戻り値の型を bool に変更
bool CollisionStrategySphereSphere::CheckCollision(
    const CollisionBase* colA,
    const CollisionBase* colB) const
{
    {
        const CollisionSphere* sphereA = dynamic_cast<const CollisionSphere*>(colA);
        const CollisionSphere* sphereB = dynamic_cast<const CollisionSphere*>(colB);

        if (!sphereA || !sphereB)
        {
            return false;
        }

        const D3DXVECTOR3& centerA = sphereA->GetWorldPosition();
        const D3DXVECTOR3& centerB = sphereB->GetWorldPosition();

        D3DXVECTOR3 diff = centerA - centerB;
        float distanceSq = D3DXVec3LengthSq(&diff);

        float radiiSum = sphereA->GetRadius() + sphereB->GetRadius();
        float radiiSumSq = radiiSum * radiiSum;

        std::cout << radiiSum << std::endl;
        std::cout << distanceSq << std::endl;

        // 衝突判定の結果を直接返す
        return distanceSq <= radiiSumSq;
    }
}

float CollisionStrategySphereSphere::GetDistanceSq(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2)
{
    D3DXVECTOR3 diff;
    // ベクトル p2 - p1 を計算
    D3DXVec3Subtract(&diff, &p2, &p1);
    // 結果ベクトルの長さの2乗を計算 (距離の2乗)
    return D3DXVec3LengthSq(&diff);

}
