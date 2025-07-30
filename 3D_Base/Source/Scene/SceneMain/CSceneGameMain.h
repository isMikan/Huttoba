#pragma once
#include "DirectX/CDirectX9.h"
#include "DirectX/CDirectX11.h"
#include "Scene/CScene.h"
#include "DebugText/CDebugText.h"
#include "Assets/Sprite/Sprite3D/CSprite3D.h"
#include "Assets/Sprite/Sprite2D/CSprite2D.h"
#include "GameObject/SpriteObject/CSpriteObject.h"
#include "GameObject/SpriteObject/Explosion/CExplosion.h"
#include "GameObject/UIObject/CUIObject.h"
#include "Assets/Mesh/StaticMesh/CStaticMesh.h"
#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"
#include "GameObject/MeshObject/StaticMesh/CCharactor/CCharacter.h"
#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/CPlayer.h"
#include "GameObject/MeshObject/StaticMesh/Ground/CGround.h"
#include "GameObject/MeshObject/StaticMesh/CCharactor/CEnemy/CEnemy.h"
#include "Assets/Mesh/SkinMesh/CSkinMesh.h"
#include "Scene/CSceneManager.h"
#include "Assets/AssetManager.h"

#include "Camera/CCamera.h"

#include<string>
#include<vector>
#include<unordered_map>
#include<memory>

class CSceneGameMain
	: public CScene
{
public:
	CSceneGameMain(HWND hWnd);
	~CSceneGameMain() override;

	HRESULT Create() override;
	HRESULT LoadData()	override;
	void Destroy() override;
	void Update() override;
	void Draw() override;


	HRESULT CreateUI();
	HRESULT CteateExplosion();
	HRESULT CreateCharactor();

	void AttachMeshToEnemy();

private:

	CDirectX9* m_pDx9;
	CDirectX11* m_pDx11;

	HWND		m_hWnd;			//ウィンドウハンドル

	std::unique_ptr<CDebugText> m_pDbgText;	//デバッグテキスト

	CCamera* m_pCamera;	//カメラ.

	//爆発クラス
	std::vector<std::unique_ptr<CExplosion> > 	m_pExplosiones;

	//UIオブジェクトクラス
	std::unordered_map<UIList, std::unique_ptr<CUIObject> > m_pUIMap;

	//キャラクタークラス
	std::unique_ptr<CPlayer>	m_pPlayer;

	//エネミークラス
	std::unordered_map < EnemyList, std::vector<std::unique_ptr<CEnemy> > > m_pEnemies;

	//地面クラス
	std::unique_ptr<CGround>	m_pGround;

};
