#include "stdafx.h"
#include "CollisionBase.h"
#include "GameObject/CGameObject.h"
#include "Collision/CollisionListener/ICollisionListener.h"

CollisionBase::CollisionBase(
    ICollisionListener*     pListener,
    const D3DXVECTOR3      syncPosition,    //\‘¢‘Ì‚¾‚¯‚ÇŽQÆ‚É‚·‚é‚Æˆê¶’Ç]‚·‚é‚Ì‚Å’l“n‚µ
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
    , m_MoveOffsetA ()
    , m_MoveOffsetB ()
{
    m_MoveOffsetA = { 0.f,0.f, 0.f };
    m_MoveOffsetB = { 0.f,0.f, 0.f };
}

void CollisionBase::SetLocalOffSetToCapsule(
    const D3DXVECTOR3& localOffsetA,
    const D3DXVECTOR3& localOffsetB)
{
    m_MoveOffsetA = localOffsetA;
    m_MoveOffsetB = localOffsetB;
}

void CollisionBase::InvokeCollision(CollisionBase* pOther)
{
    if (m_pListener)
    {
        m_pListener->OnCollision(pOther);
    }
}
