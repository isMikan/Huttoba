#pragma once

#include "GameObject/UIObject/GaugeBase/CGaugeBase.h"

#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"
#include "PlayerBase/PlayerManager/CPlayerManager.h"
#include "PlayerBase/CPlayerBase.h"

#include "Item/ItemBase.h"

#include <unordered_set>

/***********************************************************************
*	ゲージマネージャークラス.
**/
class CGaugeManager
{
public:
	CGaugeManager(ItemManager* itemManager);
	~CGaugeManager();

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 初期化処理 ---.
	void Init(CPlayerManager* playerManager);
	//--- 更新処理 ---.
	void Update();
	//--- 描画処理 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj);

protected:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- ゲージの構築処理 ---.
	void GaugeCreate(CStaticMeshObject* object, Gauge usageLimit);
	//--- 破棄処理 ---.
	void Destroy(CStaticMeshObject* object);

protected:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	std::vector<std::unique_ptr<CGaugeBase>>	m_pGauge;				//ゲージ.

	std::unordered_map<CStaticMeshObject*, std::pair<int, int>>	m_ObjectGauge;		//プレイヤーとゲージを照らし合わせる.	
	std::unordered_set<CStaticMeshObject*>						m_SubscribeObjects;	//プレイヤーの購買リスト.

	CPlayerManager*		m_pPlayerManager;		//プレイヤー.
	ItemManager*		m_pItemManager;			//アイテム.

	float				m_DeletePos;			//削除する地点.
};