#pragma once

#include <string>

class CPlayer;

class CActionState
{
public:
	CActionState() {};
	virtual ~CActionState() {};

	//状態が始めるときだけ呼び出される.
	virtual void Enter(CPlayer& pPlayer) = 0;
	//状態が終わるときだけ呼び出される.
	virtual void Exit(CPlayer& pPlayer) = 0;
	//この状態の間は毎フレーム呼び出される.
	virtual void Update(CPlayer& pPlayer) = 0;

protected:

};