#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/CPlayerBase.h"

/***********************************************************************
*	プレイヤーAIクラス.
**/
class CPlayerAI
	: public CPlayerBase	//プレイヤーベースクラスを継承.
{
public:
	CPlayerAI(int index);	//引数:プレイヤー番号.
	virtual ~CPlayerAI() override;

//======================================================================
//		外部で呼び出す関数.
//======================================================================
	//--- 毎フレームの動作 ---.
	virtual void Update() override;
	//--- 毎フレームの描画 ---.
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	void AutomaticMovement();

protected:

};