#pragma once

#include "GameObject/UIObject/Fade/FadeBase/CFadeBase.h"

/***********************************************************************
*	フェードマネージャークラス.					制作者 [甲把]
**/
class CFadeManager
{
public:
	CFadeManager();
	~CFadeManager();

	//唯一のインスタンスを作成.
	static CFadeManager& GetInstance()
	{
		static CFadeManager s_Instance;
		return s_Instance;
	}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//リセット.
	static void Reset(float alpha) {
		GetInstance().Reset_Internal(alpha);
	}
	//描画処理.
	static void Draw(
		float startTime, float duration,
		bool isFade) {	//true : フェードイン・false : フェードアウト.
		GetInstance().Draw_Internal(startTime, duration, isFade);
	}

	//α値を取得.
	static float GetAlpha() {
		return GetInstance().GetAlpha_Internal();
	}

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- 構築処理 ---.
	void Create();
	//--- 読み込み処理 ---.
	void LoadData();
	//--- リセット ---.
	void Reset_Internal(float alpha);
	//--- 描画処理 ---.
	void Draw_Internal(
		float startTime, float duration,
		bool isFade);	//true : フェードイン・false : フェードアウト.

	//--- α値をフェードベースから取得 ---.
	float GetAlpha_Internal();

private:
	std::unique_ptr<CFadeBase>	m_pFadeBase;	//フェード.

};
