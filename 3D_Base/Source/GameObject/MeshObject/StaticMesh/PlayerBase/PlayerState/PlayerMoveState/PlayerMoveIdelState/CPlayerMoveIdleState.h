#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーが移動しない状態クラス.
**/
class CPlayerMoveIdleState
	: public CPlayerState	//プレイヤー状態継承クラス.
{
public:
	CPlayerMoveIdleState(CPlayerBase& pPlayer);
	CPlayerMoveIdleState(CPlayerBase& pPlayer, float x, float z);
	~CPlayerMoveIdleState() override;

	//--- 通知 ---.
	void OnNotify(IPlayerObserver::PlayerEvent event) override;

	//--- 状態の開始時に呼び出す関数 ---.
	void Enter() override;
	//--- 状態の終了時に呼び出す関数 ---.
	void Exit() override;
	//--- この状態の間に呼び出す関数 ---.
	void Update() override;

private:
	CPlayerMoveIdleState() = delete;	//デフォルトコンストラクタ禁止.
	CPlayerMoveIdleState(const CPlayerMoveIdleState&) = delete;
	CPlayerMoveIdleState& operator = (const CPlayerMoveIdleState& rhs) = delete;

};