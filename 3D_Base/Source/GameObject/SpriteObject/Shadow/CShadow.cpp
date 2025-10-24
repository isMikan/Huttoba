#include "CShadow.h"

CShadow::CShadow()
{
}

CShadow::~CShadow()
{
}

void CShadow::Update(D3DXVECTOR3 objectPos)
{
	//‰e‚ÌˆÊ’u.
	D3DXVECTOR3 pos = objectPos;
	pos.y = 0.01f;	//’n–Ê‚ÌˆÊ’u‚É‘µ‚¦‚é.
	m_vPosition = pos;
}

void CShadow::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	m_pSprite->SetPatternNo(5,5);
	m_vRotation = D3DXVECTOR3(D3DXToRadian(90.f), 0.f, 0.f);
	m_pSprite->SetBillboard(false);
	CSpriteObject::Draw(View, Proj);
}