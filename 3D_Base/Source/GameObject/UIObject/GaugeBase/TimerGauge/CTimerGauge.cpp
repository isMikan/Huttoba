#include "CTimerGauge.h"

CTimerGauge::CTimerGauge()
{
	m_vScale = { m_GaugeSize, m_GaugeSize, 1.f };
}

CTimerGauge::~CTimerGauge()
{
}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================

//--- 更新処理 ---.
void CTimerGauge::Update()
{
	CGaugeBase::Update();
}

//--- 描画処理 ---.
void CTimerGauge::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	//ゲージ量 = ゲージ幅 * (現在の時間 / 時間の最大数).
	float w = 1.f * m_GaugeInfo.remaining / m_GaugeInfo.max;
	m_vScale = { w * m_GaugeSize, 1.f * m_GaugeSize, 1.f };	//ゲージサイズを設定.

	CGaugeBase::Draw(View, Proj);
}
