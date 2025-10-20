#include "CTimerGauge.h"

CTimerGauge::CTimerGauge()
	: m_Time			()
{
}

CTimerGauge::~CTimerGauge()
{
}

void CTimerGauge::Update()
{
	CGaugeBase::Update();
}

void CTimerGauge::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	//体力ゲージ(もう一つ ( 0 ) は台紙).
	if (m_PatternNo.y == 1)
	{
		//ゲージ量 = ゲージ幅 * (現在の時間 / 時間の最大数).
		float w = 1.f * m_Time.remaining / m_Time.max;
		SetScale(w, 1.f, 1.f);	//ゲージサイズを設定.
	}
}
