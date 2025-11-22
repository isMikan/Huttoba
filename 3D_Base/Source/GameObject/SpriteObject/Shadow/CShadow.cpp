#include "CShadow.h"

CShadow::CShadow()
	: m_DisplayHigh		( 10.f )
	, m_MaxSize			( 0.017f )
{
}

CShadow::~CShadow()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

void CShadow::Update(D3DXVECTOR3 objectPos)
{
	//オブジェクトの位置.
	D3DXVECTOR3 pos = objectPos;

	//各軸のスケール.
	float scale = m_MaxSize;
		
	//限度の高さより高い場合.
	if (objectPos.y > m_DisplayHigh)
	{
		scale = 0.f;	//表示しない.
	}
	//限度の高さ以下の場合.
	if (objectPos.y <= m_DisplayHigh)
	{
		//大きさ = 物体の高さ * 最低の大きさ（最大の大きさ / 最大の表示高さ）.	
		scale -= objectPos.y * (m_MaxSize / m_DisplayHigh);
	}
	//地面に物体がついている場合.
	if (objectPos.y <= 0.f)
	{
		scale = m_MaxSize;
	}
	m_vScale.x = scale;
	m_vScale.y = scale;

	pos.y = 0.01f;	//地面の位置に揃える.

	//影の位置を設定.
	m_vPosition = pos;
}

void CShadow::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	m_vRotation = D3DXVECTOR3(D3DXToRadian(90.f), 0.f, 0.f);

	CSpriteObject::Draw(View, Proj);
}