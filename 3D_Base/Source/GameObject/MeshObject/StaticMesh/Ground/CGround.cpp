#include "CGround.h"

CGround::CGround()
{
	m_ObjColor.diffuse =	D3DXVECTOR4( 0.f, 0.5f, 0.f, 1.f );
	m_ObjColor.ambient =	D3DXVECTOR4( 0.1f, 0.2f, 0.1f, 1.f );
	m_ObjColor.specular =	D3DXVECTOR4( 0.1f, 0.2f, 0.1f, 1.f );

	SetObjectColor(m_ObjColor);
}

CGround::~CGround()
{
}

void CGround::Update()
{
	CStaticMeshObject::Update();
}

void CGround::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj,
	LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}
