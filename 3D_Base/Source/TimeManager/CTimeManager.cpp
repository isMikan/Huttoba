#include "CTimeManager.h"

CTimeManager::CTimeManager()
{
	Reset_Internal();	//初期化.
}

CTimeManager::~CTimeManager()
{
}

//--- リセット関数 ---.
void CTimeManager::Reset_Internal()
{
	m_StartTime = Clock::now();
	m_PreviousTime = m_StartTime;

	m_DeltaTime = 0.0;
	m_TotalTime = 0.0;

	m_IsPaused = false;
}

//--- 一時停止関数 ---.
void CTimeManager::Pause_Internal()
{
	//一時停止中でない.
	if (!m_IsPaused)
	{
		m_IsPaused = true;	//一時停止開始.
	}
}

//--- 再開関数 ---.
void CTimeManager::Resume_Internal()
{
	//一時停止中.
	if (m_IsPaused)
	{
		m_PreviousTime = Clock::now();
		m_IsPaused = false;	//一時停止解除.
	}
}

//--- 更新関数 ---.
void CTimeManager::Update_Internal()
{
	//一時停止中.
	if (m_IsPaused)
	{
		m_DeltaTime = 0.0;	//デルタタイムを 0 に固定.
		return;
	}

	//現在の時間の時点.
	TimePoint currentTime = Clock::now();

	//以前のフレームからの経過時間 = 現在の時刻 - 以前の経過時間.
	std::chrono::duration<double> delta = currentTime - m_PreviousTime;

	m_DeltaTime = delta.count();	//秒単位のものを数値として取り出す.
	if (m_DeltaTime < 0.0) m_DeltaTime = 0.0;	//負の数にしない.

	m_PreviousTime = currentTime;


	//経過時間 = 現在の時刻 - 開始時間.
	std::chrono::duration<double> total = currentTime - m_StartTime;
	m_TotalTime = total.count();	//秒単位のものを数値として取り出す.
}