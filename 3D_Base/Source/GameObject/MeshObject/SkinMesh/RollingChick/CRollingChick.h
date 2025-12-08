#pragma once

#include "GameObject/MeshObject/SkinMesh/CSkinMeshObject.h"

#include "PlayerBase/CPlayerBase.h"

/***********************************************************************
*	回るひよこクラス.							制作者 [甲把]
**/
class CRollingChick
	: public CSkinMeshObject
{
public:
	CRollingChick();
	~CRollingChick() override;

//======================================================================
//		外部で呼び出す関数.
//======================================================================
	//--- 更新処理 ---.
	void Update() override;
	//--- 描画処理 ---.
	void Draw( 
		D3DXMATRIX& view, D3DXMATRIX& proj,
		LIGHT& light, CAMERA& camera ) override;

//======================================================================
//		外部で呼び出す関数.
//======================================================================
	//--- プレイヤーの位置に設定 ---.
	void SetPlayerPos(CPlayerBase* player);

private:
};
