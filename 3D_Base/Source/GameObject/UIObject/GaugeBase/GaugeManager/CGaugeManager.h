#pragma once

#include "GameObject/UIObject/GaugeBase/CGaugeBase.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerManager/CPlayerManager.h"
#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

#include <unordered_set>

/***********************************************************************
*	ゲージマネージャークラス.
**/
class CGaugeManager
{
public:
	CGaugeManager();
	~CGaugeManager();

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 構築関数 ---.
	void Create(
		CPlayerManager* playerManager);
	//--- 破棄関数 ---.
	void Destroy();
	//--- 更新関数 ---.
	void Update(
		CPlayerManager* playerManager);
	//--- 描画処理 ---.
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj);

protected:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	std::vector<std::unique_ptr<CGaugeBase>>	m_pGauge;			//ゲージ.

	std::unordered_map<CPlayerBase*, int>		m_PlayerGauge;		//プレイヤーとゲージを照らし合わせる.	
	std::unordered_set<CPlayerBase*>			m_SubscribePlayers;	//プレイヤーの購買リスト.
};