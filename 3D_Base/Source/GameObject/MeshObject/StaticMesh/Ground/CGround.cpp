#include "CGround.h"

CGround::CGround()
{
	m_Diffuse =	
		D3DXVECTOR4(0.5f, 1.0f, 0.5f, 1.0f);
	m_Ambient =
		D3DXVECTOR4(0.5f, 0.8f, 0.5f, 1.0f);
	m_Specular =
		D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
}

CGround::~CGround()
{
}

void CGround::Update()
{
}
