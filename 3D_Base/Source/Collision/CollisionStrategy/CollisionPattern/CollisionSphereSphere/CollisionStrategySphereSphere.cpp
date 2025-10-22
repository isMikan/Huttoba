#include "CollisionStrategySphereSphere.h"
#include "GameObject/CGameObject.h"
#include "Collision/Collider/CollisionSphere/CollisionSphere.h"

bool CollisionStrategySphereSphere::CheckCollision(
    const CGameObject* objA, 
    const CollisionSphere* sphereA,
    const CGameObject* objB, 
    const CollisionSphere* sphereB)
{
    {
        if (!objA || !sphereA || !objB || !sphereB)  return false;

        // 親の絶対座標 + コリダーのローカルオフセット
        D3DXVECTOR3 centerA = objA->GetPosition() + sphereA->GetLocalOffset();
        D3DXVECTOR3 centerB = objB->GetPosition() + sphereB->GetLocalOffset();

        // 中心間の距離の2乗を計算
        float distSq = GetDistanceSq(centerA, centerB);

        // 2つの半径の合計とその2乗を計算
        float radiiSum = sphereA->GetRadius() + sphereB->GetRadius();
        float radiiSumSq = radiiSum * radiiSum;

        // 距離の2乗が半径の合計の2乗以下であれば衝突
        return distSq <= radiiSumSq;
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
