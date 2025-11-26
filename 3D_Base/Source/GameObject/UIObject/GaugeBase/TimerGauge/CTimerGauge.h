#pragma once

#include "GameObject/UIObject/GaugeBase/CGaugeBase.h"

/***********************************************************************
*	時間経過型ゲージクラス.
**/
class CTimerGauge
	: public CGaugeBase
{
public:
	CTimerGauge();
	~CTimerGauge() override;

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 更新処理 ---.
	void Update() override;
	//--- 描画処理 ---.
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj) override;

};