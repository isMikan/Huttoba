#pragma once

#include "GameObject/UIObject/CUIObject.h"

/***********************************************************************
*	空クラス.									制作者 [甲把]
**/
class CSky
	: public CUIObject	//UIオブジェクトクラスを継承
{
public:
	CSky();
	~CSky() override;

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	void Update() override;
	void Draw() override;

protected:
	D3DXVECTOR2		m_MoveUV;		//動くUV.

};
