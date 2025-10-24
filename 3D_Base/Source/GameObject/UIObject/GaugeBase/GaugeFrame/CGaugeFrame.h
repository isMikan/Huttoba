#pragma once

#include "GameObject/UIObject/GaugeBase/CGaugeBase.h"

class CGaugeFrame
	: public CGaugeBase
{
public:
	CGaugeFrame();
	~CGaugeFrame() override;

	//--- XVˆ— ---.
	void Update() override;
	//--- •`‰æˆ— ---.
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) override;

private:

};