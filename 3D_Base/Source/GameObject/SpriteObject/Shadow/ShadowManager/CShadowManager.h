#pragma once

#include "GameObject/SpriteObject/Shadow/CShadow.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerManager/CPlayerManager.h"
#include "GameObject/MeshObject/StaticMesh/Item/ItemManager/ItemManager.h"

/***********************************************************************
*	影マネージャークラス
**/
class CShadowManager
{
public:
	CShadowManager();
	~CShadowManager();

	//--- 構築関数 ---.
	void Create();
	//--- 読込関数 ---.
	void LoadData();
	//--- 破棄関数 ---.
	void Destroy();
	//--- 更新関数 ---.
	void Update(CPlayerManager* player, ItemManager* item);
	//--- 描画処理 ---.
	virtual void Draw(
		CDirectX11* pDx11, D3DXMATRIX& View, D3DXMATRIX& Proj);

private:
	//影クラス.
	std::vector<std::unique_ptr<CShadow>> 		m_pShadow;

};
