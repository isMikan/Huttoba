#include "CGaugeFrame.h"

CGaugeFrame::CGaugeFrame()
{
}

CGaugeFrame::~CGaugeFrame()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 更新処理 ---.
void CGaugeFrame::Update()
{
	CGaugeBase::Update();
}

//--- 描画処理 ---.
void CGaugeFrame::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	CGaugeBase::Draw(View, Proj);
}
