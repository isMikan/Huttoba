#pragma once

class IPlayerObserver
{
public:
	enum class PlayerEvent
	{

		Attack,
		Hit,
	};

public:
	 virtual void OnNotify(PlayerEvent event) = 0;
	 virtual ~IPlayerObserver() = default;

private:

};