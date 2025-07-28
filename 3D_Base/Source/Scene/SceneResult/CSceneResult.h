#pragma once
#include "Scene/CScene.h"

class CSceneResult
	: public CScene
{
public:
	CSceneResult();
	~CSceneResult() override;

	HRESULT Create()	override;
	HRESULT LoadData()	override;
	void Update()	override;
	void Draw()		override;
	void Destroy()	override;

private:

};
