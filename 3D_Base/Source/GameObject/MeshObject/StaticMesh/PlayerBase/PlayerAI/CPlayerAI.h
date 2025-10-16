#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

/**************************************************
*	プレイヤーAIクラス.
**/
class CPlayerAI
	: public CPlayerBase	//プレイヤーベースクラスを継承.
{
public:
	CPlayerAI(int index);		//引数:コントローラ番号.
	~CPlayerAI() override;

	//----- 毎フレームの動作する関数 -----.
	void Update() override;
	//----- 毎フレームの描画する関数 -----.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

private:

};