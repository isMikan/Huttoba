#include "CollisionSphere.h"

CollisionSphere::CollisionSphere(
    float radius,
    ColliderTag tag, 
    const D3DXVECTOR3& localOffset
)
    : m_Radius(radius)
{
    // Šî’êƒNƒ‰ƒX‚Ìƒƒ“ƒo‚ğİ’è
    SetTag(tag);
    SetLocalOffset(localOffset);
}
