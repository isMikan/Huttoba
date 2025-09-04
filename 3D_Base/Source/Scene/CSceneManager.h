#pragma once
#include "Global.h"

#include<memory>

#include "CScene.h"
#include "Scene/SceneTitle/CSceneTitle.h"
#include "Scene/SceneStandby/CSceneStandby.h"
#include "Scene/SceneMain/CSceneGameMain.h"
#include "Scene/SceneResult/CSceneResult.h"
#include "CInput/CInputManager.h"

class CSceneManager final
{
public:
	CSceneManager(HWND hWnd);
	~CSceneManager();

	HRESULT Create();
	HRESULT LoadData();
	void Update();
	void Draw();
	void Destroy();

private:
	void CreateScene(int Scene);

	void SetInputBInding();

private:
	std::unique_ptr<CScene>		m_pScene;
	CInputManager&				m_pInputManager;

	HWND	m_hWnd;
};