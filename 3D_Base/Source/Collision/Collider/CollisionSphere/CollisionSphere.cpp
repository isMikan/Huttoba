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