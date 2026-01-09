#pragma once

#include "DirectX/CDirectX9.h"
#include "DirectX/CDirectX11.h"

#include "Scene/CScene.h"

#include "DebugText/CDebugText.h"
#include "Assets/Sprite/Sprite3D/CSprite3D.h"
#include "Assets/Sprite/Sprite2D/CSprite2D.h"
#include "Assets/Mesh/StaticMesh/CStaticMesh.h"
#include "Assets/Mesh/SkinMesh/CSkinMesh.h"

#include "GameObject/SpriteObject/CSpriteObject.h"
#include "GameObject/SpriteObject/DrawTimer/DrawTimer.h"
#include "GameObject/SpriteObject/Explosion/CExplosion.h"

#include "GameObject/UIObject/CUIObject.h"

#include "GameObject/SpriteObject/Shadow/ShadowManager/CShadowManager.h"
#include "GameObject/UIObject/GaugeBase/GaugeManager/CGaugeManager.h"

#include "PlayerBase/PlayerManager/CPlayerManager.h"
#include "PlayerBase/PlayerSEManager/CPlayerSEManager.h"
#include "Ground/GroundManager/CGroundManager.h"

#include "GameObject/MeshObject/SkinMesh/RollingChick/RollingChickManager/CRollingChickManager.h"

#include "Scene/CSceneManager.h"
#include "Input/CInputManager.h"

#include "GroundCollisionProxy/CGroundCollisionProxy.h"

class ItemManager;
class CollisionDraw;

class CSceneGameMain
	: public CScene
{
public:
	CSceneGameMain(HWND hWnd, std::unordered_map<ItemID, bool>& SpawnItemData);
	~CSceneGameMain() override;

	HRESULT Create() override;
	HRESULT LoadData()	override;
	void Init();
	void Destroy() override;
	void Update() override;
	void Draw() override;

	HRESULT CteateExplosion();

	void ManageEffectLaser();

private:
	CDirectX9* m_pDx9;
	CDirectX11* m_pDx11;

	HWND		m_hWnd;			//ウィンドウハンドル

	//ゲーム内の進行状況
	enum class GameState
	{
		Ready,	//ゲーム開始時のReady..GOの表示
		Play,	//ゲームプレイ
		Finish	//ゲーム終了時のFinishの表示
	};

	//現在のゲーム内の進行状況
	GameState	m_GameState;

	//進行状況の変化用タイマー
	float		m_StateTimer;

	//レディからプレイに遷移するまでの時間
	float		m_ReadyDuration;
	//フィニッシュから次の画面に遷移するまでの時間
	float		m_FinishDuration;

	std::unique_ptr<CDebugText> m_pDbgText;	//デバッグテキスト

	//爆発クラス
	std::vector<std::unique_ptr<CExplosion>> 	m_pExplosiones;

	//影クラス.
	std::unique_ptr<CShadowManager>		m_pShadowManager;

	std::unique_ptr<DrawTimer>			m_pDrawTimer;

	//ゲージマネージャークラス.
	std::unique_ptr<CGaugeManager>		m_pGaugeManager;

	//プレイヤーマネージャークラス.
	std::unique_ptr<CPlayerManager>		m_pPlayerManager;
	std::unique_ptr<CPlayerSEManager>	m_pPlayerSEManager;

	//地面マネージャークラス.
	std::unique_ptr<CGroundManager> 	m_pGroundManager;

	//アイテムクラス
	std::unique_ptr<ItemManager>		m_pItemManager;

	//ひよこマネージャークラス.
	std::unique_ptr<CRollingChickManager>	m_pRollingChickManager;

	//地面の当たり判定生成クラス
	std::unique_ptr<CGroundCollisionProxy>	m_pGroundCollisionProxy;

	//レディゴー画像
	std::unique_ptr<CUIObject>		m_pSpriteReadyGo;
	//フィニッシュ画像
	std::unique_ptr<CUIObject>		m_pSpriteFinish;

	bool	m_IsPause;	//ポーズ中か.
};
