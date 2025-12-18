#pragma once

#include "GameObject/SpriteObject/CSpriteObject.h"

/***********************************************************************
*	空クラス.									制作者 [甲把]
**/
class CSky
	: public CSpriteObject	//スプライトオブジェクトクラスを継承
{
public:
	CSky();
	~CSky() override;

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	void Update() override;
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj) override;

protected:
	float		m_DisplayHigh;		//表示する高さの限度.
	float		m_MaxSize;			//最大の大きさ.

};
