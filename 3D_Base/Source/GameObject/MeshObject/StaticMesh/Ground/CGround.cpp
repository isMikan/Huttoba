#include "CGround.h"

CGround::CGround()
	: FallSpeed		( 10.f )
{
	m_ObjColor.diffuse =	D3DXVECTOR4( 0.1f, 0.3f, 0.1f, 1.f );
	m_ObjColor.ambient =	D3DXVECTOR4( 0.1f, 0.2f, 0.1f, 1.f );
	m_ObjColor.specular =	D3DXVECTOR4( 0.1f, 0.1f, 0.1f, 1.f );
}

CGround::~CGround()
{
}

void CGround::Update()
{
	CStaticMeshObject::Update();

	float dt = CTimeManager::GetDeltaTime();

	m_vPosition.y -= dt * FallSpeed;
}

void CGround::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj,
	LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}
