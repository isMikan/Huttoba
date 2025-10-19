#pragma once

#include "Input/CInput.h"

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

/**************************************************
*	サイズ後々消す(メタセコイアでアイテム作るとき参考にしてください)
* 
*	プレイヤー	頭0.3
*				体1.0
*				手0.15
/**************************************************


/**************************************************
*	プレイヤークラス.
**/
class CPlayer
	: public CPlayerBase	//プレイヤーベースクラスを継承.
{

public:
	CPlayer(int index);		//引数はコントローラ番号.
	~CPlayer() override;

	//--- 毎フレームの動作する関数 ---.
	void Update() override;
	//--- 毎フレームの描画する関数 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	//--- キー操作 ---.
	void HandleInput();

	//--- キーバインドを設定する関数 ---.
	void SetPlayerInputBinding(int index) const;

private:

};