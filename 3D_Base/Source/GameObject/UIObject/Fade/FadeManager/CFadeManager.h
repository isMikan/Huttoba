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
	//--- リセット ---.
	void Reset(float alpha);
	//--- 構築処理 ---.
	void Create();
	//--- 読み込み処理 ---.
	void LoadData();
	//--- 描画処理 ---.
	void Draw(
		float startTime, float duration,
		bool isFade);	//true : フェードイン・false : フェードアウト.

private:
	std::unique_ptr<CFadeBase>	m_pFadeBase;	//フェード.

};
