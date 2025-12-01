#pragma once
#include "../GameStateBase.h"

class GameFinishState
	: public GameStateBase
{
public:
	GameFinishState();
	~GameFinishState();

	void Update() override;
	void Draw() override;

private:

};