#pragma once

#include "DirectX/CDirectX9.h"
#include "DirectX/CDirectX11.h"

#include "Camera/CCamera.h"
#include "Scene/CScene.h"

#include "DebugText/CDebugText.h"
#include "Assets/Sprite/Sprite3D/CSprite3D.h"
#include "Assets/Sprite/Sprite2D/CSprite2D.h"
#include "GameObject/SpriteObject/CSpriteObject.h"
#include "GameObject/SpriteObject/Explosion/CExplosion.h"
#include "GameObject/UIObject/CUIObject.h"
#include "Assets/Mesh/StaticMesh/CStaticMesh.h"
#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerManager/CPlayerManager.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include "GameObject/MeshObject/StaticMesh/Ground/GroundManager/CGroundManager.h"

#include "GameObject/UIObject/GaugeBase/GaugeManager/CGaugeManager.h"

#include "GameObject/MeshObject/StaticMesh/DrawCollision/DrawCollision.h"

#include "Assets/Mesh/SkinMesh/CSkinMesh.h"
#include "Scene/CSceneManager.h"
#include "Input/CInputManager.h"

class ItemManager;

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
	void Init();
	void Destroy() override;
	void Update() override;
	void Draw() override;

	HRESULT CreateUI();
	HRESULT CteateExplosion();
	HRESULT CreateCharactor();

	void ManageEffectLaser();

private:
	CDirectX9*	m_pDx9;
	CDirectX11* m_pDx11;

	HWND		m_hWnd;			//ウィンドウハンドル

	std::unique_ptr<CDebugText> m_pDbgText;	//デバッグテキスト

	std::unique_ptr<CCamera>	m_pCamera;	//カメラ.

	//爆発クラス
	std::vector<std::unique_ptr<CExplosion> > 	m_pExplosiones;

	//UIオブジェクトクラス
	std::unordered_map<UIList, std::unique_ptr<CUIObject> > m_pUIMap;

	//ゲージマネージャークラス.
	std::unique_ptr<CGaugeManager>		m_pGaugeManager;

	//プレイヤーマネージャークラス
	std::unique_ptr<CPlayerManager>		m_pPlayerManager;

	//地面マネージャークラス
	std::unique_ptr<CGroundManager> 	m_pGroundManager;

	//アイテムクラス
	std::unique_ptr<ItemManager>	m_pItemManager;
	
	//当たり判定描画クラス
	std::unique_ptr<DrawCollision>	m_pDrawCollision;
};
