#pragma once

#include "PlayerBase/PlayerState/CPlayerState.h"

/***********************************************************************
*	プレイヤーが回転しない状態クラス.
**/
class CPlayerTurnIdleState
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
	CPlayerTurnIdleState(CPlayerBase& pPlayer);
	~CPlayerTurnIdleState() override;

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 状態の開始時に呼び出す ---.
	void Enter() override {};
	//--- 状態の終了時に呼び出す ---.
	void Exit() override {};
	//--- この状態の間に呼び出す ---.
	void Update() override {};

private:
//======================================================================
// 	   呼び出しを禁止するコンストラクタ.
//======================================================================
	CPlayerTurnIdleState() = delete;	//デフォルトコンストラクタ削除.
	CPlayerTurnIdleState(const CPlayerTurnIdleState&) = delete;
	CPlayerTurnIdleState& operator = (const CPlayerTurnIdleState& rhs) = delete;

};