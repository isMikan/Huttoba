#pragma once

#include "PlayerBase/PlayerAI/CPlayerAI.h"

/***********************************************************************
*	プレイヤーAI : Aタイプクラス.			制作者 [甲把]
**/
class CPlayerAI_TypeA
	: public CPlayerAI	//プレイヤーベースクラスを継承.
{
public:
	CPlayerAI_TypeA(int index);	//引数:プレイヤー番号.
	~CPlayerAI_TypeA() override;

//======================================================================
//		外部で呼び出す関数.
//======================================================================
	//--- 毎フレームの動作 ---.
	void Update() override;
	//--- 毎フレームの描画 ---.
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

private:
	bool IsSearchPlayer(CPlayerBase* player);

	bool IsSearchItem(ItemBase* item);

};