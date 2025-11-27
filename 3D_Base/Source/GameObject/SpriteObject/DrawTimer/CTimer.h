#pragma once
#include <chrono>		//高精度タイマーや時間管理用のライブラリ.
#include <iostream>


class CTimer
{
public:
	CTimer();
	~CTimer();

	//シーンが開始してから呼ぶ関数.
	void Start();
	//経過時間を秒単位で取得.
	float GetElapsedSeconds() const;
	//停止.
	void Stop();
	//再生.
	void RStart();

private:
	//high_resolution_clock：最も精度の高い時間を提示してくれる.
	//time_point：ある時点の時間を表してくれる.
	std::chrono::high_resolution_clock::time_point m_StartTime;
	//タイマーが動作中かどうか.
	bool m_TimerFlag;
};