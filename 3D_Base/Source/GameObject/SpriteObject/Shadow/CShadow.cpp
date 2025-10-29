#include "CShadow.h"

CShadow::CShadow()
{
}

CShadow::~CShadow()
{
}

void CShadow::Update(D3DXVECTOR3 objectPos)
{
	//オブジェクトの位置.
	D3DXVECTOR3 pos = objectPos;

	float scale = 0.017f;
		
	if (objectPos.y > 10.f)
	{
		scale = 0.f;
	}
	if (objectPos.y <= 10.f)
	{
		scale -= objectPos.y * 0.0015f;
	}
	if (objectPos.y <= 0)
	{
		scale = 0.017f;
	}
	std::cout << "スケール" << scale << std::endl;
	m_vScale.x = scale;
	m_vScale.y = scale;

	pos.y = 0.01f;	//地面の位置に揃える.

	//影の位置を設定.
	m_vPosition = pos;
}

void CShadow::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	m_pSprite->SetPatternNo(5, 5);
	m_vRotation = D3DXVECTOR3(D3DXToRadian(90.f), 0.f, 0.f);
	m_pSprite->SetBillboard(false);

	CSpriteObject::Draw(View, Proj);
}