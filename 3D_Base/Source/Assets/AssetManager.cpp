#include "stdafx.h"
#include "AssetManager.h"

AssetManager::AssetManager()
	: m_pEffectManager	(nullptr)
	, m_pMeshManager	(nullptr)
	, m_pSoundManager	(nullptr)
	, m_pSpriteManager	(nullptr)
{
}

AssetManager::~AssetManager()
{
}


//--------------------------------------------------------------------------------------------------------------

HRESULT AssetManager::Create()
{
	//マネージャーのインスタンス作成
	m_pEffectManager = std::make_shared<CEffect>();			if (!m_pEffectManager)	return E_FAIL;
	m_pMeshManager	 = std::make_shared<MeshManager>();		if (!m_pMeshManager)	return E_FAIL;
	m_pSoundManager  = std::make_shared<CSoundManager>();	if (!m_pSoundManager)	return E_FAIL;
	m_pSpriteManager = std::make_shared<SpriteManager>();	if (!m_pSpriteManager)	return E_FAIL;

	//マネージャーで管理している各種アセットの作成
	m_pEffectManager->Create(CDirectX11::GetInstance()->GetDevice(),CDirectX11::GetInstance()->GetContext());
	m_pMeshManager	->Create();
	m_pSoundManager	->Create();
	m_pSpriteManager->Create();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

HRESULT AssetManager::LoadData()
{
	//マネージャーで管理している各種アセットの読み込み
	if(m_pEffectManager->LoadData() != S_OK) return E_FAIL;
	if(m_pMeshManager  ->LoadData() != S_OK) return E_FAIL;
	if(m_pSoundManager ->LoadData(CCreateWindow::GetInstance()->GetHundle()) != S_OK) return E_FAIL;
	if(m_pSpriteManager->LoadData() != S_OK) return E_FAIL;

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
