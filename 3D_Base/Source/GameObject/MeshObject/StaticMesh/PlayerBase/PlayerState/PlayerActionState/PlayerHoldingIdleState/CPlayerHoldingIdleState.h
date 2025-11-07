#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/***********************************************************************
*	プレイヤーが何も行動していない状態クラス（アイテム所持版）.
**/
class CPlayerHoldingIdleState
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
	CPlayerHoldingIdleState(CPlayerBase& pPlayer);
	~CPlayerHoldingIdleState() override;

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

//=== 手の調整位置 ===.
	D3DXVECTOR3		m_HoldBothHands_RightOffsetPos;	//両手で持つアイテムの右.
	D3DXVECTOR3		m_HoldBothHands_LeftOffsetPos;	//両手で持つアイテムの左.

//====================.

private:
//======================================================================
// 	   呼び出しを禁止するコンストラクタ.
//======================================================================
	CPlayerHoldingIdleState() = delete;	//デフォルトコンストラクタ削除.
	CPlayerHoldingIdleState(const CPlayerHoldingIdleState&) = delete;
	CPlayerHoldingIdleState& operator = (const CPlayerHoldingIdleState& rhs) = delete;

};