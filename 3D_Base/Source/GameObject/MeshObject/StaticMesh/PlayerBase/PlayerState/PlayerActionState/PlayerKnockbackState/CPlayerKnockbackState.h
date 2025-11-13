#pragma once

#include "PlayerBase/PlayerState/CPlayerState.h"

/***********************************************************************
*	プレイヤーの吹き飛ばされた状態クラス.
**/
class CPlayerKnockbackState
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
	CPlayerKnockbackState(CPlayerBase& pPlayer);
	~CPlayerKnockbackState() override;

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
	D3DXVECTOR3		m_UpHandOffset;			//上に調整.
	D3DXVECTOR3		m_StartRightAxis;		//開始時の右軸.
	D3DXVECTOR3		m_Velocity;				//初速度.	

	float			m_StartTime;			//開始時間.
	float			m_EndTime;				//終了時間.

	float			m_Gravity;				//重力.
	float			m_RotateSpeed;			//回転する速度.
	float			m_CurrentTiltAngle;		//現在の傾き角度.

	D3DXQUATERNION	m_StartQuat;			//開始時の回転.

private:
//======================================================================
// 	   呼び出しを禁止するコンストラクタ.
//======================================================================
	CPlayerKnockbackState() = delete;	//デフォルトコンストラクタ削除.
	CPlayerKnockbackState(const CPlayerKnockbackState&) = delete;
	CPlayerKnockbackState& operator = (const CPlayerKnockbackState& rhs) = delete;

};