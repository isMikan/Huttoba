#include "CFadeManager.h"

CFadeManager::CFadeManager()
	: m_pFadeBase		()
{
	Create();
	LoadData();
}

CFadeManager::~CFadeManager()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 構築処理 ---.
void CFadeManager::Create()
{
	//フェードクラスのインスタンス作成.
	m_pFadeBase = std::make_unique<CFadeBase>();
}

//--- 読み込み処理 ---.
void CFadeManager::LoadData()
{
	//フェードスプライトを設定.
	m_pFadeBase->AttachSprite(AssetManager::Sprite(Sprite2DList::Fade));
}

//--- リセット ---.
void CFadeManager::Reset_Internal(float alpha)
{
	m_pFadeBase->Reset(alpha);
}

//--- 描画処理 ---.
void CFadeManager::Draw_Internal(
	float startTime, float duration,
	bool isFade)	//true : フェードイン・false : フェードアウト.
{
	if (isFade)
	{
		m_pFadeBase->FadeIn(startTime, duration);
	}
	else
	{
		m_pFadeBase->FadeOut(startTime, duration);
	}

	m_pFadeBase->Draw();
}

//--- α値をフェードベースから取得 ---.
float CFadeManager::GetAlpha_Internal()
{
	return m_pFadeBase->GetAlpha();
}
