#include "CPlayerHead.h"

CPlayerHead::CPlayerHead()
	: m_OffsetPos		( 0.f, 1.f, 0.f )
{
	m_ObjColor.diffuse =	D3DXVECTOR4( 0.8f, 0.8f, 0.8f, 1.f );
	m_ObjColor.ambient =	D3DXVECTOR4( 0.3f, 0.3f, 0.3f, 1.f );
	m_ObjColor.specular =	D3DXVECTOR4( 0.1f, 0.1f, 0.1f, 1.f );
}

CPlayerHead::~CPlayerHead()
{
}

void CPlayerHead::Update()
{
	CStaticMeshObject::Update();
}

void CPlayerHead::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}
