#include "CGame.h"
#include "Assets/Sound/CSoundManager.h"
#include "Assets/Effect/CEffect.h"
#include <ranges> 
#include<memory>

//コンストラクタ.
CGame::CGame(HWND hWnd )
	: m_hWnd			(hWnd)
	, m_pSceneManager	()
{
	Create();
}


//デストラクタ.
CGame::~CGame()
{
}

//構築.
void CGame::Create()
{
	m_pSceneManager = std::make_unique<CSceneManager>(m_hWnd);
	m_pSceneManager->Create();
}

//ロードデータ関数.
HRESULT CGame::LoadData()
{
	//サウンドデータの読み込み
	if (CSoundManager::GetInstance()->Load(m_hWnd) == false) {
		return E_FAIL;
	}

	return S_OK;

	int a;
}

//解放関数.
void CGame::Release()
{
}


//更新処理.
void CGame::Update()
{
	m_pSceneManager->Update();
}

//描画処理.
void CGame::Draw()
{
	m_pSceneManager->Draw();
}


