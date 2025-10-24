#pragma once

#include "GameObject/UIObject/GaugeBase/CGaugeBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerManager/CPlayerManager.h"

class CGaugeManager
{
public:
	CGaugeManager();
	~CGaugeManager();

	//--- \’zŠÖ” ---.
	void Create();
	//--- “ÇŠÖ” ---.
	void LoadData(
		CPlayerManager* playerManager);
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

};