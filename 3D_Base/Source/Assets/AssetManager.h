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

	/*****************************************************************************************
	* @brief    唯一のインスタンスを作成、取得する
	*****************************************************************************************/
	static AssetManager* GetInstance()
	{
		static AssetManager instance;
		return &instance;
	}


	/*****************************************************************************************
	* @brief    各マネージャーのインスタンス生成、各アセットの登録
	*****************************************************************************************/
	HRESULT Create();


	/*****************************************************************************************
	* @brief    登録したアセットデータを読み込む
	*****************************************************************************************/
	HRESULT LoadData();


	/*****************************************************************************************
	* @brief    エフェクトマネージャーのインスタンスを取得する
	* @return   エフェクトマネージャーのインスタンス
	*****************************************************************************************/
	static std::shared_ptr<CEffect>	Effect()  { return AssetManager::GetInstance()->m_pEffectManager; }


	/*****************************************************************************************
	* @brief    サウンドマネージャーのインスタンスを取得する
	* @return   サウンドマネージャーのインスタンス
	*****************************************************************************************/
	static std::shared_ptr<CSoundManager> Sound() { return AssetManager::GetInstance()->m_pSoundManager; }


	/*****************************************************************************************
	* @brief    StaticMeshList型、SkinMeshList型で指定したメッシュのデータを取得する
	* @return   指定したメッシュのデータ
	* @note		オーバーロードしている関数
	*****************************************************************************************/
	static std::shared_ptr<CStaticMesh>	Mesh(StaticMeshList meshId)  { return AssetManager::GetInstance()->m_pMeshManager->GetStaticMesh(meshId); }
	static std::shared_ptr<CSkinMesh>	Mesh(SkinMeshList meshId) { return AssetManager::GetInstance()->m_pMeshManager->GetSkinMesh(meshId); }

	/*****************************************************************************************
	* @brief    Sprite2DList型、Sprite3DList型で指定したメッシュのデータを取得する
	* @return   指定したメッシュのデータ
	* @note		オーバーロードしている関数
	*****************************************************************************************/
	static std::shared_ptr<CSprite2D> Sprite(Sprite2DList spriteId) { return AssetManager::GetInstance()->m_pSpriteManager->GetSprite2D(spriteId); }
	static std::shared_ptr<CSprite3D> Sprite(Sprite3DList spriteId)  { return AssetManager::GetInstance()->m_pSpriteManager->GetSprite3D(spriteId); }

private:
	AssetManager();
	~AssetManager();

	// コピーコンストラクタ、代入演算子を禁止
	AssetManager(const AssetManager& rhs) = delete;
	AssetManager& operator=(const AssetManager& rhs) = delete;

	std::shared_ptr<CEffect>		m_pEffectManager;	// エフェクトマネージャー
	std::shared_ptr<MeshManager>	m_pMeshManager;		// メッシュマネージャー
	std::shared_ptr<CSoundManager>	m_pSoundManager;	// サウンドマネージャー
	std::shared_ptr<SpriteManager>	m_pSpriteManager;	// スプライトマネージャー

};