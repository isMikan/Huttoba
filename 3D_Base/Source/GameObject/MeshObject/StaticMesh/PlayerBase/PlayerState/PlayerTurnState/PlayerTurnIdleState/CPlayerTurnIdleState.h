#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーが回転しない状態クラス.
**/
class CPlayerTurnIdleState
	: public CPlayerState	//プレイヤー状態継承クラス.
{
public:
	CPlayerTurnIdleState(CPlayerBase& pPlayer);
	CPlayerTurnIdleState(CPlayerBase& pPlayer, float x, float z);
	~CPlayerTurnIdleState() override;

	//--- 状態の開始時に呼び出す関数 ---.
	void Enter() override;
	//--- 状態の終了時に呼び出す関数 ---.
	void Exit() override;
	//--- この状態の間に呼び出す関数 ---.
	void Update() override;

private:
	CPlayerTurnIdleState() = delete;	//デフォルトコンストラクタ禁止.
	CPlayerTurnIdleState(const CPlayerTurnIdleState&) = delete;
	CPlayerTurnIdleState& operator = (const CPlayerTurnIdleState& rhs) = delete;

};