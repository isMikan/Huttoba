#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーが何も行動していない状態クラス.
**/
class CPlayerActionIdleState
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
	CPlayerActionIdleState(CPlayerBase& pPlayer);
	~CPlayerActionIdleState() override;

	//--- 状態の開始時に呼び出す関数 ---.
	void Enter() override;
	//--- 状態の終了時に呼び出す関数 ---.
	void Exit() override;
	//--- この状態の間に呼び出す関数 ---.
	void Update() override;

private:
	CPlayerActionIdleState() = delete;	//デフォルトコンストラクタ禁止.
	CPlayerActionIdleState(const CPlayerActionIdleState&) = delete;
	CPlayerActionIdleState& operator = (const CPlayerActionIdleState& rhs) = delete;

};