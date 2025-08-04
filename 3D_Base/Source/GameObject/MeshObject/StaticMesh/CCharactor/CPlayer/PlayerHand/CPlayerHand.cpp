#include "CPlayerHand.h"

CPlayerHand::CPlayerHand()
	: m_OffsetPos		( 0.f, 0.f, 0.f )
{
}

CPlayerHand::~CPlayerHand()
{
}

void CPlayerHand::Update()
{
	CCharacter::Update();
}

void CPlayerHand::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CCharacter::Draw(View, Proj, Light, Camera);
}