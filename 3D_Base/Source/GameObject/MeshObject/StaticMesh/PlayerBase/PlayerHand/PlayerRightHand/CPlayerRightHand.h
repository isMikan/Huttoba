#pragma once

#include "PlayerBase/PlayerHand/CPlayerHand.h"

/***********************************************************************
*	プレイヤーの右手クラス.
**/
class CPlayerRightHand
	: public CPlayerHand	//プレイヤーの手を継承.
{
public:
	CPlayerRightHand();
	~CPlayerRightHand();

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 毎フレームの動作 ---.
	void Update() override;
	//--- 毎フレームの描画 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

private:

};