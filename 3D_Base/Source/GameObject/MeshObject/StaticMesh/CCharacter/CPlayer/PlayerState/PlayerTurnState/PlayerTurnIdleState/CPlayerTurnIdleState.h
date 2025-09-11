#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーが回転しない状態クラス.
**/
class CPlayerTurnIdleState
	: public CPlayerState	//プレイヤー状態継承クラス.
{
public:
	CPlayerTurnIdleState();
	CPlayerTurnIdleState(float x, float z);
	~CPlayerTurnIdleState() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;

private:

};