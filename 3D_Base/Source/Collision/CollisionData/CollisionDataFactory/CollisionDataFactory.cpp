#include "stdafx.h"
#include "CollisionDataFactory.h"

std::shared_ptr<CollisionBase> CollisionDataFactory::CreateSphere(
    CGameObject* pOwner,
    float radius,
    CollisionBase::ColliderTag tag, 
    const D3DXVECTOR3& localOffset
)
{
    // 親オブジェクトがICollisionListenerを実装しているかチェックし、ポインタを取得
    ICollisionListener* listener = dynamic_cast<ICollisionListener*>(pOwner);

    // 親オブジェクトからワールド位置へのconst参照を取得
    const D3DXVECTOR3& posRef = pOwner->GetPosition();

    // shared_ptrを使って具象クラス (CollisionSphere) のインスタンスを生成
    // 生成と同時に、参照とタグを渡して初期化する
    std::shared_ptr<CollisionBase> newCollider = std::make_shared<CollisionSphere>(
        listener,        // 親のポインタ
        posRef,          // 親の座標(参照で初期化してる変数)
        tag,             // 識別タグ
        radius,          // 半径
        localOffset      // 微調整用
    );

    return newCollider; // 所有権を呼び出し元（CollisionManager）に移譲

}
