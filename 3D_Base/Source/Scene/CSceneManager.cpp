#include "CSceneManager.h"

CSceneManager::CSceneManager(HWND hWnd)
	: m_pScene		( nullptr )
	, m_hWnd		( hWnd )
{
	Create();
}

CSceneManager::~CSceneManager()
{
}

HRESULT CSceneManager::Create()
{
	m_pScene = std::make_unique<CSceneTitle>();

	return S_OK;
}

HRESULT CSceneManager::LoadData()
{

	return S_OK;
}

void CSceneManager::Update()
{
	//シーンが変更されてるか？
	if (m_pScene->GetChangetScene() == true)
	{
		//次のシーンを作成
		CreateScene(m_pScene->GetNextScene());
	}

	m_pScene->Update();
}

void CSceneManager::Draw()
{
	m_pScene->Draw();
}

void CSceneManager::Destroy()
{
}

void CSceneManager::CreateScene(int Scene)
{
	//前のシーンを削除
	m_pScene->Destroy();

	//シーン作成
	switch (Scene)
	{
	case Title:		m_pScene = std::make_unique<CSceneTitle>	();	break;
	case GameMain:  m_pScene = std::make_unique<CSceneGameMain>	(m_hWnd);	break;
	case GameOver:  m_pScene = std::make_unique<CSceneGameOver>	();	break;
	case Ending:	m_pScene = std::make_unique<CSceneResult>	();	break;
	default: 
		//終了
		return;
		break;
	}
	//シーン変更確認用フラッグをfalseに
	m_pScene->ChangeSceneFlagFalse();
}
