#pragma once

#include <chrono>

class CTimeManager
{
public:
	CTimeManager();
	~CTimeManager();

	//唯一のインスタンスを作成.
	static CTimeManager& Instance()
	{
		static CTimeManager s_Instance;
		return s_Instance;
	}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//リセット関数を外部へ.
	static void Reset() { Instance().Reset_Internal(); }
	//一時停止関数を外部へ.
	static void Pause() { Instance().Pause_Internal(); }
	//再開関数を外部へ.
	static void Resume() { Instance().Resume_Internal(); }
	//更新関数を外部へ.
	static void Update() { Instance().Update_Internal(); }

//======================================================================
		
	//デルタタイムを取得.
	static float GetDeltaTime() { return static_cast<float>(Instance().m_DeltaTime); }
	//経過時間を取得.
	static float GetTotalTime() { return static_cast<float>(Instance().m_TotalTime); }

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- リセット ---.
	void Reset_Internal();
	//--- 一時停止（ポーズ開始） ---.
	void Pause_Internal();
	//--- 再開（ポーズ終了） ---.
	void Resume_Internal();
	//--- 更新処理 ---.
	void Update_Internal();

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	//長いので using で短くする.
	using Clock = std::chrono::high_resolution_clock;	//高精度タイマー.
	using TimePoint = std::chrono::time_point<Clock>;	//ある時点.

	TimePoint	m_StartTime;	//開始時間.
	TimePoint	m_PreviousTime;	//以前の時間.

	double		m_DeltaTime;	//1フレームの間どれだけ経ったか.
	double		m_TotalTime;	//経過時間.

	bool		m_IsPaused;		//一時停止しているか.
};