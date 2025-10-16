#include "CPlayerAI.h"
#include "Sound/CSoundManager.h"

CPlayerAI::CPlayerAI(int index)
	: CPlayerBase	( index )
{
}

CPlayerAI::~CPlayerAI()
{
}

//----- 毎フレームの動作する関数 -----.
void CPlayerAI::Update()
{
	CPlayerBase::Update();
}

//----- 毎フレームの描画する関数 -----.
void CPlayerAI::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera )
{
	CPlayerBase::Draw( View, Proj, Light, Camera );
}