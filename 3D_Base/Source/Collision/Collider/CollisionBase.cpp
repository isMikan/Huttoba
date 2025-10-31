#include "stdafx.h"
#include "CollisionBase.h"
#include "GameObject/CGameObject.h"
#include "Collision/CollisionListener/ICollisionListener.h"

CollisionBase::CollisionBase(
    ICollisionListener*     pListener,
    const D3DXVECTOR3&      syncPosition,
    ColliderTag             tag,
    const D3DXVECTOR3&      localOffset
)
    : m_pListener   (pListener)
    , m_SyncPosition(syncPosition)
    , m_Tag         (tag)
    , m_LocalOffset (localOffset)
    , m_WorldPosition(0.0f, 0.0f, 0.0f)
    , m_IsActive    (true)
    , m_IsHit       (false)
{
}

void CollisionBase::InvokeCollision(CollisionBase* pOther)
{
    if (m_pListener)
    {
        m_pListener->OnCollision(pOther);
    }
}
