#include "CPlayerHead.h"

CPlayerHead::CPlayerHead()
	: m_OffsetPos		( 0.f, 1.f, 0.f )
{
}

CPlayerHead::~CPlayerHead()
{
}

void CPlayerHead::Update()
{
	CCharacter::Update();
}

void CPlayerHead::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CCharacter::Draw(View, Proj, Light, Camera);
}
