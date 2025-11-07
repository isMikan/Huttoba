#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/***********************************************************************
*	プレイヤーが何も行動していない状態クラス.
**/
class CPlayerActionIdleState
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
	CPlayerActionIdleState(CPlayerBase& pPlayer);
	~CPlayerActionIdleState() override;

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//--- 状態の開始時に呼び出す ---.
	void Enter() override;
	//--- 状態の終了時に呼び出す ---.
	void Exit() override;
	//--- この状態の間に呼び出す ---.
	void Update() override;

private:
	//======================================================================
	// 	   内部で使用する変数.
	//======================================================================
	float			m_StartTime;			//開始時間.
	float			m_EndTime;				//終了時間.

private:
//======================================================================
// 	   呼び出しを禁止するコンストラクタ.
//======================================================================
	CPlayerActionIdleState() = delete;	//デフォルトコンストラクタ削除.
	CPlayerActionIdleState(const CPlayerActionIdleState&) = delete;
	CPlayerActionIdleState& operator = (const CPlayerActionIdleState& rhs) = delete;

};