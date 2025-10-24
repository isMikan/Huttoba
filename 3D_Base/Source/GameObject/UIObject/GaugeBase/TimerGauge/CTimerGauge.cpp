#include "CTimerGauge.h"

CTimerGauge::CTimerGauge()
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
	//ゲージ量 = ゲージ幅 * (現在の時間 / 時間の最大数).
	float w = 1.f * m_GaugeInfo.remaining / m_GaugeInfo.max;
	SetScale(w, 1.f, 1.f);	//ゲージサイズを設定.

	CGaugeBase::Draw(View, Proj);
}
