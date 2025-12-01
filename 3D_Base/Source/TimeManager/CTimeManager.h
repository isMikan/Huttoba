#pragma once

#include <chrono>

/***********************************************************************
*	タイムマネージャークラス.					制作者 [甲把]
**/
class CTimeManager
{
public:
	CTimeManager();
	~CTimeManager();

	//唯一のインスタンスを作成.
	static CTimeManager& GetInstance()
	{
		static CTimeManager s_Instance;
		return s_Instance;
	}

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//リセット関数を外部へ.
	static void Reset() { GetInstance().Reset_Internal(); }
	//一時停止関数を外部へ.
	static void Pause() { GetInstance().Pause_Internal(); }
	//再開関数を外部へ.
	static void Resume() { GetInstance().Resume_Internal(); }
	//更新関数を外部へ.
	static void Update() { GetInstance().Update_Internal(); }

//======================================================================
		
	//デルタタイムを取得.
	static float GetDeltaTime() { return static_cast<float>(GetInstance().m_DeltaTime); }
	//経過時間を取得.
	static float GetTotalTime() { return static_cast<float>(GetInstance().m_TotalTime); }

	//製作者 : 佐藤
	//残り制限時間を取得
	static std::pair<int, int> GetTimeLimit() { return GetInstance().m_TimeLimit; }
	//制限時間が過ぎたかどうか
	bool GetIsFinish() const { return m_IsFinish; }

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- リセット ---.
	void Reset_Internal();
	//--- 一時停止関数 ---.
	void Pause_Internal();
	//--- 再開関数 ---.
	void Resume_Internal();
	//--- 更新処理 ---.
	void Update_Internal();

	//製作者 : 佐藤
	//--- 残り時間計算 ---.
	void Calculate_TimeLimit();

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	//長いので using で短くする.
	using Clock = std::chrono::high_resolution_clock;	//高精度タイマー.
	using TimePoint = std::chrono::time_point<Clock>;	//ある時点.

	TimePoint	m_StartTime;		//開始時間.
	TimePoint	m_PreviousTime;		//以前の時間.
	TimePoint	m_PauseStateTime;	//ポーズ開始時間.

	double		m_DeltaTime;	//1フレームの間どれだけ経ったか.
	double		m_TotalTime;	//経過時間.

	double		m_PauseTime;	//ポーズ時間.

	bool		m_IsPaused;		//一時停止しているか.
	bool		m_IsFinish;		//一時停止しているか.

	//製作者 : 佐藤
	std::pair<int, int> m_TimeLimit;	//残り制限時間
};