#pragma once

#include "GameObject/UIObject/GaugeBase/CGaugeBase.h"

class CGaugeManager
{
public:
	CGaugeManager();
	~CGaugeManager();

	//--- \’zŠÖ” ---.
	void Create();
	//--- “ÇŠÖ” ---.
	void LoadData();
	//--- ”jŠüŠÖ” ---.
	void Destroy();
	//--- XVŠÖ” ---.
	void Update();
	//--- •`‰æˆ— ---.
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj);

protected:
	std::unique_ptr<CGaugeBase>		m_pGauge;	//ƒQ[ƒW.

};