#include "CTimer.h"

CTimer::CTimer()
	: m_TimerFlag		( false )
{
}

CTimer::~CTimer()
{
}

//シーンが開始してから呼ぶ関数.
void CTimer::Start()
{
	//現在の正確な時刻を取得※タイマーの基準点を設定.
	m_StartTime = std::chrono::high_resolution_clock::now();
	//動作中.
	m_TimerFlag = true;
}

//経過時間を秒単位で取得.
float CTimer::GetElapsedSeconds() const
{
	//タイマーが動いてなければ経過時間は0秒.
	if (m_TimerFlag != true)
	{
		return 0.0f;
	}
	//毎フレーム呼ぶことで、現在の時間を取得.
	std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
	//経過時間計算.
	std::chrono::duration<float> elapsed = now - m_StartTime;
	//float値として経過時間を返す.
	return elapsed.count();
}

//停止.
void CTimer::Stop()
{
	//動作していない.
	m_TimerFlag = false;
}

void CTimer::RStart()
{
	//動作中.
	m_TimerFlag = true;
}
