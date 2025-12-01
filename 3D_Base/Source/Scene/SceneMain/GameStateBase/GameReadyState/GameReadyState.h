#pragma once
#include "../GameStateBase.h"

class GameReadyState
	: public GameStateBase
{
public:
	GameReadyState();
	~GameReadyState();

	void Update() override;
	void Draw() override;

private:

};