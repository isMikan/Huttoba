#pragma once
#include "Scene/CScene.h"

class CSceneGameOver
	: public CScene
{
public:
	CSceneGameOver();
	~CSceneGameOver() override;

	HRESULT Create() override;
	HRESULT LoadData()	override;
	void Update() override;
	void Draw() override;
	void Destroy() override;


private:

};
