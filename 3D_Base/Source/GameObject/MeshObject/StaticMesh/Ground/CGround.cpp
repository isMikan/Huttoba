#include "CGround.h"

CGround::CGround()
{
	m_Diffuse = D3DXVECTOR4(0.5f, 1.f, 0.5f, 1.f);
	m_Ambient = D3DXVECTOR4(0.5f, 1.f, 0.5f, 1.f);
	m_Specular = D3DXVECTOR4(0.5f, 1.f, 0.5f, 1.f);
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
