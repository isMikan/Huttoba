#include "CPlayerHand.h"

CPlayerHand::CPlayerHand()
	: m_OffsetPos		( 0.f, 0.f, 0.f )
{
	m_ObjColor.diffuse =	D3DXVECTOR4( 0.8f, 0.8f, 0.8f, 1.f );
	m_ObjColor.ambient =	D3DXVECTOR4( 0.3f, 0.3f, 0.3f, 1.f );
	m_ObjColor.specular =	D3DXVECTOR4( 0.1f, 0.1f, 0.1f, 1.f );
}

CPlayerHand::~CPlayerHand()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 毎フレームの動作 ---.
void CPlayerHand::Update()
{
	CStaticMeshObject::Update();
}

//--- 毎フレームの描画 ---.
void CPlayerHand::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}