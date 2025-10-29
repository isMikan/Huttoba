#include "CollisionStrategySphereSphere.h"
#include "GameObject/CGameObject.h"
#include "Collision/Collider/CollisionSphere/CollisionSphere.h"

bool CollisionStrategySphereSphere::CheckCollision(
    const CollisionBase* colA,
    const CollisionBase* colB) const
{
    {
        //スフィアにできるか確認
        const CollisionSphere* sphereA = dynamic_cast<const CollisionSphere*>(colA);
        const CollisionSphere* sphereB = dynamic_cast<const CollisionSphere*>(colB);

        if (!sphereA || !sphereB)
        {
            return false;
        }

        //スフィアの位置を取得
        const D3DXVECTOR3& centerA = sphereA->GetWorldPosition();
        const D3DXVECTOR3& centerB = sphereB->GetWorldPosition();

        //二点間の距離を引く
        D3DXVECTOR3 diff = centerA - centerB;
        //距離の二乗
        float distanceSq = D3DXVec3LengthSq(&diff);

        //半径を足す
        float radiiSum = sphereA->GetRadius() + sphereB->GetRadius();
        //半径の二乗
        float radiiSumSq = radiiSum * radiiSum;

        // 距離の2乗と半径の２乗を比べる
        return distanceSq <= radiiSumSq;
    }
}
