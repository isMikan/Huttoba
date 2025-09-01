#include "CGameObject.h"

//コンストラクタ.
CGameObject::CGameObject()
	: m_vPosition	()
	, m_vRotation	()
	, m_vScale		( 1.0f, 1.0f, 1.0f )

	, m_vQuaternion	( D3DXQUATERNION(0.f, 0.f, 0.f, 1.f) )
{
}

//デストラクタ.
CGameObject::~CGameObject()
{
}