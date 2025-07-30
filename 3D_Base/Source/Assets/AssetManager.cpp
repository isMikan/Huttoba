#include "stdafx.h"
#include "AssetManager.h"

AssetManager::AssetManager()
	: m_pEffectManager(nullptr)
	, m_pMeshManager(nullptr)
	, m_pSoundManager(nullptr)
	, m_pSpriteManager(nullptr)
{
}

AssetManager::~AssetManager()
{
}

HRESULT AssetManager::Create()
{
	m_pEffectManager = std::make_shared<CEffect>(); if (!m_pEffectManager) return E_FAIL;
	m_pMeshManager = std::make_shared<MeshManager>(); if (!m_pMeshManager) return E_FAIL;
	m_pSoundManager = std::make_shared<CSoundManager>(); if (!m_pSoundManager) return E_FAIL;
	m_pSpriteManager = std::make_shared<SpriteManager>(); if (!m_pSpriteManager) return E_FAIL;

	m_pEffectManager->Create(CDirectX11::GetInstance()->GetDevice(),CDirectX11::GetInstance()->GetContext());
	m_pMeshManager->Create();
	m_pSoundManager->Create();
	m_pSpriteManager->Create();

	return S_OK;
}

HRESULT AssetManager::LoadData()
{
	m_pEffectManager->LoadData();
	m_pMeshManager->LoadData();
	m_pSoundManager->Load(CCreateWindow::GetInstance()->GetHundle());
	m_pSpriteManager->LoadData();
	return S_OK;
}