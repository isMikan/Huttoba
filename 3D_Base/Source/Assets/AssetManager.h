#pragma once

#include"Effect/CEffect.h"
#include"Mesh/MeshManager.h"
#include"Mesh/StaticMesh/CStaticMesh.h"
#include"Sound/CSoundManager.h"
#include"Sprite/SpriteManager.h"
#include"Main/CCreateWindow.h"

class AssetManager
{
public:

	static AssetManager* GetInstance()
	{
		static AssetManager instance;
		return &instance;
	}

	HRESULT Create();
	HRESULT LoadData();

	static std::shared_ptr<CEffect>	Effect()  { return AssetManager::GetInstance()->m_pEffectManager; }
	static std::shared_ptr<CSoundManager> Sound() { return AssetManager::GetInstance()->m_pSoundManager; }

	static std::shared_ptr<CStaticMesh>	Mesh(StaticMeshList meshId)  { return AssetManager::GetInstance()->m_pMeshManager->GetStaticMesh(meshId); }
	static std::shared_ptr<CSkinMesh>	Mesh(SkinMeshList meshId) { return AssetManager::GetInstance()->m_pMeshManager->GetSkinMesh(meshId); }

	static std::shared_ptr<CSprite2D> Sprite(Sprite2DList spriteId) { return AssetManager::GetInstance()->m_pSpriteManager->GetSprite2D(spriteId); }
	static std::shared_ptr<CSprite3D> Sprite(Sprite3DList spriteId)  { return AssetManager::GetInstance()->m_pSpriteManager->GetSprite3D(spriteId); }

private:
	AssetManager();
	~AssetManager();

	AssetManager(const AssetManager& rhs) = delete;
	AssetManager& operator=(const AssetManager& rhs) = delete;

	std::shared_ptr<CEffect>		m_pEffectManager;
	std::shared_ptr<MeshManager>	m_pMeshManager;
	std::shared_ptr<CSoundManager>	m_pSoundManager;
	std::shared_ptr<SpriteManager>	m_pSpriteManager;

};