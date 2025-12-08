#pragma once

#include "GameObject/MeshObject/SkinMesh/RollingChick/CRollingChick.h"

#include "PlayerBase/PlayerManager/CPlayerManager.h"
#include "PlayerBase/CPlayerBase.h"

#include <unordered_set>

/***********************************************************************
*	回るひよこマネージャークラス.				制作者 [甲把]
**/
class CRollingChickManager
{
public:
	CRollingChickManager(
		std::unique_ptr<CPlayerManager>& playerManager);
	~CRollingChickManager();

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 構築関数 ---.
	void Create(
		std::unique_ptr<CPlayerManager>& playerManager);
	//--- 更新関数 ---.
	void Update();
	//--- 描画関数 ---.
	void Draw(
		D3DXMATRIX& view, D3DXMATRIX& proj,
		LIGHT& light, CAMERA& camera);

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	std::vector<std::unique_ptr<CRollingChick>>		m_pRollingChicks;	//ひよこ.

	std::unordered_map<CPlayerBase*, int>	m_PlayerChick;	//プレイヤーとひよこを照らし合わせる.	
	std::unordered_set<CPlayerBase*>	m_SubscribePlayer;	//プレイヤーの購買リスト.

};