#pragma once

#include "PlayerBase/PlayerState/CPlayerState.h"

/***********************************************************************
*	プレイヤーが押される状態クラス.
**/
class CPlayerPushedState
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
	CPlayerPushedState(CPlayerBase& pPlayer);
	~CPlayerPushedState() override;

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

	float			m_CurrentTiltAngle;		//現在の傾き角度.
	float			m_TiltAngleMax;			//最大の傾き角度.
	float			m_PhaseSplit;			//傾きの変わり目.
	float			m_HandLaps;				//手の周回.

	D3DXVECTOR3		m_RightHandStartPos;	//右手の元の位置.
	D3DXVECTOR3		m_LeftHandStartPos;		//左手の元の位置.
	D3DXVECTOR3		m_RightHandEndPos;		//右手の元の位置.
	D3DXVECTOR3		m_LeftHandEndPos;		//左手の元の位置.

	D3DXQUATERNION	m_StartQuat;			//開始時の回転.

private:
//======================================================================
// 	   呼び出しを禁止するコンストラクタ.
//======================================================================
	CPlayerPushedState() = delete;	//デフォルトコンストラクタ削除.
	CPlayerPushedState(const CPlayerPushedState&) = delete;
	CPlayerPushedState& operator = (const CPlayerPushedState& rhs) = delete;

};