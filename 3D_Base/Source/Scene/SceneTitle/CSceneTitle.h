#pragma once
#include "Scene/CScene.h"

class CSceneTitle
	: public CScene
{
public:
	CSceneTitle();
	~CSceneTitle() override;

	HRESULT Create() override;
	HRESULT LoadData()	override;
	void Update() override;
	void Draw() override;
	void Destroy() override;

private:

};
