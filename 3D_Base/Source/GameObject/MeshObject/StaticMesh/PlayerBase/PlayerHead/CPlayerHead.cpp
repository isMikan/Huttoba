#include "CPlayerHead.h"

CPlayerHead::CPlayerHead()
	: m_OffsetPos		( 0.f, 1.f, 0.f )
{
	ObjectColor color = {
		D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.f),
		D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.f),
		D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.f) };
	//サイズを指定する.
	m_ObjectColor.resize(2);
	//色を設定.
	m_ObjectColor[0] = color;
}

CPlayerHead::~CPlayerHead()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 毎フレームの動作 ---.
void CPlayerHead::Update()
{
	CStaticMeshObject::Update();
}

//--- 毎フレームの描画 ---.
void CPlayerHead::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}