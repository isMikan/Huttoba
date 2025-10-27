#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

/***********************************************************************
*	プレイヤーAIクラス.
**/
class CPlayerAI
	: public CPlayerBase	//プレイヤーベースクラスを継承.
{
public:
	CPlayerAI(int index);		//引数:コントローラ番号.
	~CPlayerAI() override;

//======================================================================
//		外部で呼び出す関数.
//======================================================================
	//--- 毎フレームの動作 ---.
	void Update() override;
	//--- 毎フレームの描画 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

private:

};