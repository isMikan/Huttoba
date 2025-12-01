#pragma once
#include "../GameStateBase.h"

class GamePlayState
	: public GameStateBase
{
public:
	GamePlayState();
	~GamePlayState();

	void Update() override;
	void Draw() override;

private:

};