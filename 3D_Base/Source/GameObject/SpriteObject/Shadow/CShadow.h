#pragma once

#include "GameObject/SpriteObject/CSpriteObject.h"

/***********************************************************************
*	影クラス.									制作者 [甲把]
**/
class CShadow
	: public CSpriteObject	//スプライトオブジェクトクラスを継承
{
public:
	CShadow();
	~CShadow() override;

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	void Update(
		D3DXVECTOR3 objectPos);
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj) override;

protected:
	//使用しない override 関数.
	void Update() override {};

protected:
	float		m_DisplayHigh;		//表示する高さの限度.
	float		m_MaxSize;			//最大の大きさ.

};
