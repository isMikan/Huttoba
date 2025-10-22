#include "stdafx.h"
#include "CollisionBase.h"
#include "GameObject/CGameObject.h"
#include "Collision/CollisionListener/ICollisionListener.h"

CollisionBase::CollisionBase()
        : m_vLocalOffset(0.0f, 0.0f, 0.0f)
        , m_Owner(nullptr)
        , m_Tag(ColliderTag::None)
        , m_IsActive(true)
        , m_IsHit(false)
{
	
}
