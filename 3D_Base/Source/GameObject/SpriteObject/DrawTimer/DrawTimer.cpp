#include"DrawTimer.h"

DrawTimer::DrawTimer()
{
}

DrawTimer::~DrawTimer()
{
}

void DrawTimer::Draw()
{
	//secがBOSSLimit以上なら、ボス戦終了
	if (sec.count() - 1 >= (BossLimit + PoseSecond))
	{
		m_pBoss->Escape();
		TimerFlag = false;
	}

	//タイマー表示用
	TimerHundred = ((PoseSecond + BossLimit) - static_cast<int>(sec.count())) / 100;
	TimerHundredRem = ((PoseSecond + BossLimit) - static_cast<int>(sec.count())) % 100;
	TimerTen = TimerHundredRem / 10;
	TimerOne = TimerHundredRem % 10;

}
