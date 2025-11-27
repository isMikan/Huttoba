#pragma once
#include "TimeManager/CTimeManager.h"
#include "GameObject/UIObject/CUIObject.h"

class DrawTimer
{
public:

	enum class Time
	{
		Ten,
		One
	};

	DrawTimer();
	~DrawTimer();

	void Init();
	void Update();
	void Draw();

private:

	std::unique_ptr<CUIObject> m_TimerImage;
	std::unordered_map<Time, std::unique_ptr<CUIObject>> RemainingTimeImage;
	std::unordered_map<Time, int> CalculateTime;
};