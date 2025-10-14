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
	~IPlayerObserver() = default;
	void OnNotify(PlayerEvent event) = 0;

private:

};