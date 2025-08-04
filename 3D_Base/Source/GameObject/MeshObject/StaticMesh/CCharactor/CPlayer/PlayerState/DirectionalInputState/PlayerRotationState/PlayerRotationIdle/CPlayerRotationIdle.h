#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/DirectionalInputState/CPlayerDirectionalInputState.h"

class CPlayerRotationIdle
	: public CPlayerDirectionalInputState
{
public:
	CPlayerRotationIdle();
	~CPlayerRotationIdle() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;

	//スティックかカーソル入力を感知.
	void KeyInput(CPlayer& pPlayer, float x, float z) override;

	std::string GetStateName() const override;


private:

};