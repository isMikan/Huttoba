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

//--- リセット ---.
void CFadeManager::Reset(float alpha)
{
	m_pFadeBase->Reset(alpha);
}

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

//--- 描画処理 ---.
void CFadeManager::Draw(
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