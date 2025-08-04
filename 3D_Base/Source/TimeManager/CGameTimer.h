#pragma once

#include <Windows.h>

/************************************************************
*	実際の経過時間のクラス.
**/
class CTimeManager
{
public:
	//シングルトン.
	static CTimeManager* GetInstance()
	{
		static CTimeManager s_Instance;
		return &s_Instance;
	}

	~CTimeManager();

	//毎フレーム呼び出す処理.
	void Tick();

	//時間をリセット.
	void Reset() {
		QueryPerformanceCounter(&m_PreviousTime);
		QueryPerformanceCounter(&m_StartTime);
		m_CurrentTime = m_PreviousTime;
		m_DeltaTime = 0.f;
	}

	//前のフレームからの経過時間を取得.
	float GetDeltaTime() const { return m_DeltaTime; }

	//ゲーム開始からの総計時間を取得.
	float GetTotalTime() const {
		return static_cast<float>(m_CurrentTime.QuadPart - m_StartTime.QuadPart) / static_cast<float>(m_Frequency.QuadPart);
	}

private:
	CTimeManager();

private:
	//LARGE_INTEGER 高精度にするため、64ビット整数を持つ構造体を使う.
	LARGE_INTEGER	m_Frequency;	//周波数(カウントの速さ).
	LARGE_INTEGER	m_PreviousTime;	//前の時間.
	LARGE_INTEGER	m_CurrentTime;	//現在の時間
	LARGE_INTEGER	m_StartTime;	//ゲーム開始の時間

	float			m_DeltaTime;	//経過時間.
};