#include "CGround.h"

CGround::CGround()
{
#if 1
	m_ObjColor.diffuse =	D3DXVECTOR4( 0.2f, 0.8f, 0.6f, 1.f );
	m_ObjColor.ambient =	D3DXVECTOR4( 0.1f, 0.2f, 0.1f, 1.f );
	m_ObjColor.specular =	D3DXVECTOR4( 0.1f, 0.2f, 0.1f, 1.f );
#else
	//m_ObjColor.diffuse = D3DXVECTOR4(1.f, 0.f, 0.f, 1.f);
	//m_ObjColor.ambient = D3DXVECTOR4(0.2f, 0.1f, 0.1f, 1.f);
	//m_ObjColor.specular = D3DXVECTOR4(0.2f, 0.1f, 0.1f, 1.f);

	//m_ObjColor.diffuse = D3DXVECTOR4(0.7f, 0.f, 0.2f, 1.f);
	//m_ObjColor.ambient = D3DXVECTOR4(0.5f, 0.f, 0.2f, 1.f);
	//m_ObjColor.specular = D3DXVECTOR4(0.3f, 0.f, 0.1f, 1.f);

	//m_ObjColor.diffuse = D3DXVECTOR4(1.f, 0.3f, 0.1f, 1.f);
	//m_ObjColor.ambient = D3DXVECTOR4(0.5f, 0.2f, 0.2f, 1.f);
	//m_ObjColor.specular = D3DXVECTOR4(0.3f, 0.1f, 0.1f, 1.f);

	m_ObjColor.diffuse = D3DXVECTOR4(0.8f, 0.f, 0.5f, 1.f);
	m_ObjColor.ambient = D3DXVECTOR4(0.1f, 0.f, 0.f, 1.f);
	m_ObjColor.specular = D3DXVECTOR4(0.1f, 0.f, 0.f, 1.f);

	m_ObjColor.diffuse = D3DXVECTOR4(0.4f, 0.f, 0.f, 1.f);
	m_ObjColor.ambient = D3DXVECTOR4(0.3f, 0.f, 0.f, 1.f);
	m_ObjColor.specular = D3DXVECTOR4(0.2f, 0.f, 0.f, 1.f);

#endif
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
