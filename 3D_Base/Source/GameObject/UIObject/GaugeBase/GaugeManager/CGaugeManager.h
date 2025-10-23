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
	void Update(
		CPlayerManager* playerManager);
	//--- •`‰æˆ— ---.
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj);

protected:
	std::vector<std::unique_ptr<CGaugeBase>>	m_pGauge;	//ƒQ[ƒW.

	bool	m_IsSubscribe;			//“o˜^‚µ‚½‚©.

};