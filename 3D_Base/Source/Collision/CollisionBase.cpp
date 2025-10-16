#include "stdafx.h"
#include "CollisionBase.h"

CollisionBase::CollisionBase()
	: m_vPosition	{}
	, m_vRotation	{}
	, m_vScale		{}
	, Owner			{ nullptr }
	, Tag			{ ColliderTag::Unknown }
	, IsActive		{ false }
{
	
}

CollisionBase::~CollisionBase()
{
}
