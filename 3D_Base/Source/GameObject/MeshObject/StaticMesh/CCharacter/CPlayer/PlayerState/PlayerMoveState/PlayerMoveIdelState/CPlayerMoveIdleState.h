#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharacter/CPlayer/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーが移動しない状態クラス.
**/
class CPlayerMoveIdleState
	: public CPlayerState	//プレイヤー状態継承クラス.
{
public:
	CPlayerMoveIdleState();
	CPlayerMoveIdleState(float x, float z);
	~CPlayerMoveIdleState() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;

private:
};