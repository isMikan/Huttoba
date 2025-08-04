#include "CPlayerRightHand.h"

CPlayerRightHand::CPlayerRightHand()
{
	m_OffsetPos = D3DXVECTOR3( 0.4f, 0.55f, 0.f );
}

CPlayerRightHand::~CPlayerRightHand()
{
}

void CPlayerRightHand::Update()
{
	CPlayerHand::Update();
}

void CPlayerRightHand::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CPlayerHand::Draw(View, Proj, Light, Camera);
}
