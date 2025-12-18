#include "CPlayerRightHand.h"

CPlayerRightHand::CPlayerRightHand()
{
	m_OffsetPos = D3DXVECTOR3( 0.5f, 0.8f, 0.f );
}

CPlayerRightHand::~CPlayerRightHand()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 毎フレームの動作 ---.
void CPlayerRightHand::Update()
{
	CPlayerHand::Update();
}

//--- 毎フレームの描画 ---.
void CPlayerRightHand::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CPlayerHand::Draw(View, Proj, Light, Camera);
}
