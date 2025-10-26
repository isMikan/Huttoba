#pragma once

#include "GameObject/UIObject/GaugeBase/CGaugeBase.h"

/***********************************************************************
*	ゲージフレームクラス.
**/
class CGaugeFrame
	: public CGaugeBase
{
public:
	CGaugeFrame();
	~CGaugeFrame() override;

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 更新処理 ---.
	void Update() override;
	//--- 描画処理 ---.
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) override;

private:

};