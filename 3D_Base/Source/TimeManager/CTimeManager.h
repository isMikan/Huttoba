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
	//更新関数を外部へ.
	static void Update() { GetInstance().Update_Internal(); }

//======================================================================
		
	//デルタタイムを取得.
	static float GetDeltaTime() { return static_cast<float>(GetInstance().m_DeltaTime); }
	//経過時間を取得.
	static float GetTotalTime() { return static_cast<float>(GetInstance().m_TotalTime); }

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- リセット ---.
	void Reset_Internal();
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
};