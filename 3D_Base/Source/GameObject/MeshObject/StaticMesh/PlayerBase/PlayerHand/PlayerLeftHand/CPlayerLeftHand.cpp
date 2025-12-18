#include "CPlayerLeftHand.h"

CPlayerLeftHand::CPlayerLeftHand()
{
	m_OffsetPos = D3DXVECTOR3( -0.55f, 0.85f, 0.f );
}

CPlayerLeftHand::~CPlayerLeftHand()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 毎フレームの動作 ---.
void CPlayerLeftHand::Update()
{
	CPlayerHand::Update();
}

//--- 毎フレームの描画 ---.
void CPlayerLeftHand::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CPlayerHand::Draw(View, Proj, Light, Camera);
}