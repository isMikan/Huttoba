#include "CGame.h"
#include "Assets/Sound/CSoundManager.h"
#include "Assets/Effect/CEffect.h"
#include <ranges> 
#include<memory>

//コンストラクタ.
CGame::CGame(HWND hWnd )
	: m_hWnd				( hWnd )
	, m_pSceneManager		()

	, m_pDx9				( nullptr )
	, m_pDx11				( nullptr )

	, m_pStaticMeshPlayer	( nullptr )

	, m_pCamera				( nullptr )

	, m_pPlayer				( nullptr )
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

	m_pStaticMeshPlayer = new CStaticMesh();

	m_pCamera = new CCamera();

	m_pPlayer = new CPlayer();
}

//ロードデータ関数.
HRESULT CGame::LoadData()
{
	m_pDx9 = CDirectX9::GetInstance();
	m_pDx11 = CDirectX11::GetInstance();

	//サウンドデータの読み込み
	if (CSoundManager::GetInstance()->Load(m_hWnd) == false) {
		return E_FAIL;
	}

	m_pStaticMeshPlayer->Init( *m_pDx9, *m_pDx11,
		_T( "Data\\Mesh\\Static\\Character\\character.x" ) );

	m_pPlayer->AttachMesh( *m_pStaticMeshPlayer );
	m_pPlayer->SetPosition(0.f, 1.f, 6.f);

	return S_OK;
}

//解放関数.
void CGame::Release()
{
}


//更新処理.
void CGame::Update()
{
	m_pSceneManager->Update();

	m_pPlayer->Update();
}

//描画処理.
void CGame::Draw()
{
	m_pCamera->Update();

	CAMERA camera = m_pCamera->GetCamera();
	LIGHT light = m_pCamera->GetLight();
	D3DXMATRIX mView = m_pCamera->GetView();
	D3DXMATRIX mProj = m_pCamera->GetProj();

	m_pSceneManager->Draw();

	m_pPlayer->Draw(mView, mProj, light, camera);
}


