#pragma once

#include "GameObject/UIObject/GaugeBase/CGaugeBase.h"

class CTimerGauge
	: public CGaugeBase
{
public:
	CTimerGauge();
	~CTimerGauge() override;

	//--- XVˆ— ---.
	void Update() override;

	//--- •`‰æˆ— ---.
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) override;

private:
	Gauge	m_Time;		//ŠÔî•ñ.
};