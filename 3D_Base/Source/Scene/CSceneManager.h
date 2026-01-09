#pragma once
#include "Global.h"

#include "CScene.h"
#include "Scene/SceneTitle/CSceneTitle.h"
#include "Scene/SceneStandby/CSceneStandby.h"
#include "Scene/SceneMain/CSceneGameMain.h"
#include "Scene/SceneResult/CSceneResult.h"
#include "Input/CInputManager.h"

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

	CDirectX11* m_pDx11;

	HWND	m_hWnd;

	//‹óƒNƒ‰ƒX.
	std::unique_ptr<CSky> 		m_pSky;

	std::unordered_map<ItemID, bool> m_SpawnItemData;
};