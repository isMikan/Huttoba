#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/DirectionalInputState/CPlayerDirectionalInputState.h"

/**************************************************
*	プレイヤーが移動しない状態クラス.
**/
class CPlayerMoveIdle
	: public CPlayerDirectionalInputState	//入力数値の受け取り状態クラスを継承.
{
public:
	CPlayerMoveIdle();
	~CPlayerMoveIdle() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;

	//スティックかカーソル入力を感知.
	void KeyInput(CPlayer& pPlayer, float x, float z) override;
private:

};