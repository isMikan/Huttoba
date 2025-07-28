#pragma once
#include "Global.h"
#include "MyMacro.h"
class CScene
{
public:
	CScene();
	virtual~CScene();

	virtual HRESULT Create()	= 0;
	virtual HRESULT LoadData()	= 0;

	virtual void Destroy()	= 0;
	virtual void Update()	= 0;
	virtual void Draw()		= 0;


	//シーンをマネージャーに渡す
	int GetNextScene() { return m_NextScene; }
	bool GetChangetScene() { return ChangeScene; }

	//各シーンから次のシーンをセット
	void SetNextScene(int scene) 
	{ 
		m_NextScene = scene; 
		ChangeScene = true;
	}

	void ChangeSceneFlagFalse() { ChangeScene = false; }

private:

	int m_NextScene;
	bool ChangeScene;
};