#pragma once
#include "Global.h"

#include<memory>

#include "CScene.h"
#include "Scene/SceneOver/CSceneGameOver.h"
#include "Scene/SceneMain/CSceneGameMain.h"
#include "Scene/SceneTitle/CSceneTitle.h"
#include "Scene/SceneResult/CSceneResult.h"

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

	std::unique_ptr<CScene> m_pScene;

	void CreateScene(int Scene);

	HWND	m_hWnd;
};