#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/ActionState/CActionState.h"

class CPlayerActionIdle
	: public CActionState
{
public:
	CPlayerActionIdle();
	~CPlayerActionIdle() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;
	//入力されたら切り替える処理を呼び出す.
	void Handle(CPlayer& pPlayer, int inputKey) override;

	std::string GetStateName() const override;

private:

};