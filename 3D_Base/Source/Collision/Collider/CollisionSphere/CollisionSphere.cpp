#include "CollisionSphere.h"

CollisionSphere::CollisionSphere(
    ICollisionListener* pListener,
    const D3DXVECTOR3&  syncPosition,
    ColliderTag         tag,
    float               radius,
    const D3DXVECTOR3&  localOffset
) 
    : CollisionBase(pListener, syncPosition, tag, localOffset),
      m_Radius(radius)
{
}

void CollisionSphere::UpdateWorldPosition()
{
    CGameObject* obj = dynamic_cast<CGameObject*>(GetListener());
    if (!obj) { return; }

    const D3DXMATRIX world = obj->GetWorldMatrix();
    const D3DXVECTOR3 local = m_LocalOffset;

    D3DXVec3TransformCoord(
        &m_WorldPosition, // スフィアのワールド中心
        &local,
        &world
    );
}