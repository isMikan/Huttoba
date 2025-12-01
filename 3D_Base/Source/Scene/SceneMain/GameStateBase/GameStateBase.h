#pragma once

class GameStateBase
{
public:
	GameStateBase();
	~GameStateBase();

	virtual void Update() = 0;

	virtual void Draw() = 0;

private:

};