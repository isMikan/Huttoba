#include "CTimeManager.h"

CTimeManager::CTimeManager()
	: m_DeltaTime	( 0.f )
{
	//QueryPerformanceFrequency 周波数(1秒あたり何カウントか).
	QueryPerformanceFrequency(&m_Frequency);
	Reset();
}

CTimeManager::~CTimeManager()
{
}

void CTimeManager::Tick()
{
	//QueryPerformanceCounter PCの経過時間を計算してくれる.
	QueryPerformanceCounter(&m_CurrentTime);

	//QuadPart 秒数に変換してくれる.
	//秒単位の時間差 = 経過したカウント数 / 一秒あたりのカウント数
	m_DeltaTime = static_cast<float>(m_CurrentTime.QuadPart - m_PreviousTime.QuadPart)
		/ static_cast<float>(m_Frequency.QuadPart);

	//現在の時間を前の時間に設定する.
	m_PreviousTime = m_CurrentTime;

	//マイナスにはならない.
	if (m_DeltaTime < 0.f)
	{
		m_DeltaTime = 0.f;
	}
}
