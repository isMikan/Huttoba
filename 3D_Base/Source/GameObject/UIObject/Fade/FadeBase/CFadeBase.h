#pragma once

#include "GameObject/UIObject/CUIObject.h"

/***********************************************************************
*	フェードクラス.								制作者 [甲把]
**/
class CFadeBase
	: public CUIObject
{
public:
	CFadeBase();
	~CFadeBase() override;

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- α値をリセットする ---.
	void Reset(float alpha);
	//--- 更新処理 ---.
	void Update() override;
	//--- 描画処理 ---.
	void Draw() override;

	//--- 徐々に明るくなる ---.
	void FadeIn(
		float startTime, float duration);	//引数 : 開始時間・フェードにかける時間.
	//--- 徐々に暗くなる ---.
	void FadeOut(
		float startTime, float duration);	//引数 : 開始時間・フェードにかける時間.

private:

};