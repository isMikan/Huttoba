#include "CFadeBase.h"

CFadeBase::CFadeBase()
	: m_Alpha		( 255.f )
{
}

CFadeBase::~CFadeBase()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- α値をリセットする ---.
void CFadeBase::Reset(float alpha)
{
	m_Alpha = alpha;
	m_pSprite->SetAlpha(m_Alpha);
}

//--- 更新処理 ---.
void CFadeBase::Update()
{
	CUIObject::Update();
}

//--- 描画処理 ---.
void CFadeBase::Draw()
{
	m_pSprite->SetAlpha(m_Alpha);

	CUIObject::Draw();
}

//--- 徐々に明るくなる ---.
void CFadeBase::FadeIn(float startTime, float duration)
{
	if (m_Alpha < 0.f)
	{
		m_Alpha = 0.f;
		return;
	}

	if (!m_pSprite) return;

	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	//フェード時間の割合.
	float progress = (t - startTime) / duration;
	progress = std::clamp(progress, 0.f, 1.f);

	m_Alpha = 1.f - progress;
}

//--- 徐々に暗くなる ---.
void CFadeBase::FadeOut(float startTime, float duration)
{
	if (m_Alpha > 255.f)
	{
		m_Alpha = 255.f;
		return;
	}

	if (!m_pSprite) return;

	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	//フェード時間の割合.
	float progress = (t - startTime) / duration;
	progress = std::clamp(progress, 0.f, 1.f);
	
	m_Alpha = progress;
}