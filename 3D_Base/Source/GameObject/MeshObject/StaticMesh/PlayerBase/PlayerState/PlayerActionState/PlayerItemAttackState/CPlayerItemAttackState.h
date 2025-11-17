#pragma once

#include "PlayerBase/PlayerState/CPlayerState.h"

/***********************************************************************
*	プレイヤーがアイテム未所持の攻撃状態クラス.
**/
class CPlayerItemAttackState
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	CPlayerItemAttackState(CPlayerBase& pPlayer);
	~CPlayerItemAttackState();

	//--- 状態の開始時に呼び出す ---.
	void Enter() override;
	//--- 状態の終了時に呼び出す ---.
	void Exit() override;
	//--- この状態の間に呼び出す ---.
	void Update() override;

private:
//======================================================================
// 	   内部で呼び出す関数.
//======================================================================
	//--- 入力を受け付けたか判断する ---.
	bool IsInput(int index) const;

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	float			m_StartTime;			//開始時間.
	float			m_EndTime;				//終了時間.

	float			m_CurrentTiltAngle;		//現在の傾き角度.
	float			m_TiltAngleMax;			//最大の傾き角度.
	float			m_HandLaps;				//手の周回.

//=== 手の調整位置 ===.

	//開始の位置.
	D3DXVECTOR3		m_RightHandStartPos;	//右手の元の位置.
	D3DXVECTOR3		m_LeftHandStartPos;		//左手の元の位置.
	D3DXVECTOR3		m_RightHandEndPos;		//右手の目的の位置.
	D3DXVECTOR3		m_LeftHandEndPos;		//左手の目的の位置.

	//両手で持つアイテム用.
	D3DXVECTOR3		m_HoldBothHands_RightHandEndPos;	//右手の目的の位置.
	D3DXVECTOR3		m_HoldBothHands_LeftHandEndPos;		//左手の目的の位置.
	//片手で持つアイテム用.
	D3DXVECTOR3		m_OneHand_RightHandEndPos;			//右手の目的の位置.
	D3DXVECTOR3		m_OneHand_LeftHandEndPos;			//左手の目的の位置.

//====================.

	D3DXQUATERNION	m_StartQuat;			//開始時の回転.

private:
//======================================================================
// 	   呼び出しを禁止するコンストラクタ.
//======================================================================
	CPlayerItemAttackState() = delete;	//デフォルトコンストラクタ削除.
	CPlayerItemAttackState(const CPlayerItemAttackState&) = delete;
	CPlayerItemAttackState& operator = (const CPlayerItemAttackState& rhs) = delete;

};