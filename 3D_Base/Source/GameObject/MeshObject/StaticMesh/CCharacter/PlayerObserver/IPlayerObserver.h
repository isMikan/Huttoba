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
	void OnNotify(PlayerEvent event);
	~IPlayerObserver();


private:

};