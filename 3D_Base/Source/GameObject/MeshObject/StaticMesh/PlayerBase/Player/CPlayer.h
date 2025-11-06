#pragma once

#include "Input/CInput.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

/***********************************************************************
*	サイズ後々消す(メタセコイアでアイテム作るとき参考にしてください)
* 
*	プレイヤー	頭0.3
*				体1.0
*				手0.15
/***********************************************************************


/***********************************************************************
*	プレイヤークラス.							制作者 : 甲把
**/
class CPlayer
	: public CPlayerBase
{

public:
	CPlayer(int index);		//引数:コントローラ番号.
	~CPlayer() override;

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 毎フレームの動作 ---.
	void Update() override;
	//--- 毎フレームの描画 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	//--- 入力操作 ---.
	void HandleInput();

	//--- キーバインドを設定する ---.
	void SetPlayerInputBinding(int index) const;

private:
	D3DXVECTOR3		m_CurrentInput;	//現在の入力方向.

};