#include "CPlayerLeftHand.h"

CPlayerLeftHand::CPlayerLeftHand()
{
	m_OffsetPos = D3DXVECTOR3( -0.4f, 0.55f, 0.f );
}

CPlayerLeftHand::~CPlayerLeftHand()
{
}

void CPlayerLeftHand::Update()
{
	CPlayerHand::Update();
}

void CPlayerLeftHand::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CPlayerHand::Draw(View, Proj, Light, Camera);
}
