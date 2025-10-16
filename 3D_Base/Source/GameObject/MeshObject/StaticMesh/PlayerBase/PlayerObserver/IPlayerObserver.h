#pragma once

class IPlayerObserver
{
public:
	enum class HitEvent
	{

		Attack,
		Hit,
	};

public:
	 virtual void OnNotify(HitEvent event) = 0;
	 virtual ~IPlayerObserver() = default;

private:

};