#pragma once

#include "CPlayerMoveState.h"

class CPlayerMoveIdle
	: public CPlayerMoveState
{
public:
	CPlayerMoveIdle();
	~CPlayerMoveIdle() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Eixt(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;
	//入力されたら切り替える処理を呼び出す.
	void Handle(CPlayer& pPlayer, int inputKey) override;

	//テスト用のキー処理.
	void KeyInput(CPlayer& pPlayer, float x, float z) override;

	std::string GetStateName() const override;


private:

};