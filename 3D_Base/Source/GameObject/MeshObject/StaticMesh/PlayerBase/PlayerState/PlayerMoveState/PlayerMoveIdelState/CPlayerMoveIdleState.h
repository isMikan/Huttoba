#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/***********************************************************************
*	プレイヤーが移動しない状態クラス.
**/
class CPlayerMoveIdleState
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
	CPlayerMoveIdleState(CPlayerBase& pPlayer);
	CPlayerMoveIdleState(CPlayerBase& pPlayer, float x, float z);	//引数:移動方向のx軸、z軸.
	~CPlayerMoveIdleState() override;

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
	CPlayerMoveIdleState() = delete;	//デフォルトコンストラクタ削除.
	CPlayerMoveIdleState(const CPlayerMoveIdleState&) = delete;
	CPlayerMoveIdleState& operator = (const CPlayerMoveIdleState& rhs) = delete;

};