#include "CGaugeFrame.h"

CGaugeFrame::CGaugeFrame()
{
}

CGaugeFrame::~CGaugeFrame()
{
}

void CGaugeFrame::Update()
{
	CGaugeBase::Update();
}

void CGaugeFrame::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	CGaugeBase::Draw(View, Proj);
}
