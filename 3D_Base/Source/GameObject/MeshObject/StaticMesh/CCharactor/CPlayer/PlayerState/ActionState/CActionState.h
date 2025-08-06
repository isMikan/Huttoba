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
	//入力されたら切り替える処理を呼び出す.
	virtual void Handle(CPlayer& pPlayer,int inputKey) = 0;

	virtual std::string GetStateName() const = 0;

protected:

};