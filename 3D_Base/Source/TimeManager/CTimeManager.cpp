#include "CTimeManager.h"

CTimeManager::CTimeManager()
{
	Reset_Internal();	//初期化.
}

CTimeManager::~CTimeManager()
{
}

//======================================================================
// 	   内部で呼び出す関数.
//======================================================================

//--- リセット ---.
void CTimeManager::Reset_Internal()
{
	m_StartTime = Clock::now();
	m_PreviousTime = m_StartTime;

	m_DeltaTime = 0.0;
	m_TotalTime = 0.0;
}

//--- 更新処理 ---.
void CTimeManager::Update_Internal()
{
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