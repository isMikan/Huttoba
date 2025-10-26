#include "CPlayerAI.h"
#include "Sound/CSoundManager.h"

CPlayerAI::CPlayerAI(int index)
	: CPlayerBase	( index )
{
}

CPlayerAI::~CPlayerAI()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 毎フレームの動作 ---.
void CPlayerAI::Update()
{
	CPlayerBase::Update();
}

//--- 毎フレームの描画 ---.
void CPlayerAI::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera )
{
	CPlayerBase::Draw( View, Proj, Light, Camera );
}