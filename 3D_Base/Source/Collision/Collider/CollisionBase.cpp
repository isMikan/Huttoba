#include "stdafx.h"
#include "CollisionBase.h"
#include "GameObject/CGameObject.h"
#include "Collision/CollisionListener/ICollisionListener.h"

CollisionBase::CollisionBase()
	: m_vPosition	{}
	, m_vRotation	{}
	, m_vScale		{}
	, m_Owner			{ nullptr }
	, m_Tag			{ ColliderTag::None }
	, m_IsActive		{ false }
{
	
}

CollisionBase::~CollisionBase()
{
}
