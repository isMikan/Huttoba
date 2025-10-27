#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/***********************************************************************
*	プレイヤーの落ちる状態クラス.
**/
class CPlayerFallingState
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
	CPlayerFallingState(CPlayerBase& pPlayer);
	~CPlayerFallingState() override;

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
// 	   内部で呼び出す関数.
//======================================================================
	//--- ワールドでの角度を求める関数 ---.
	float WorldAngle();

	//--- この状態を終了する関数 ---.
	bool IsEnd();

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	D3DXVECTOR3		m_UpHandOffset;			//上に調整.
	D3DXVECTOR3		m_StartRightAxis;		//開始時の右軸.
	D3DXVECTOR3		m_Velocity;				//初速度.	

	float			m_StartTime;			//開始時間.
	float			m_EndTime;				//終了時間.

	float			m_GroundRange;			//地面の許容範囲.
	float			m_RotateRangeMax;		//回転の最大許容範囲.
	float			m_RotateRangeMin;		//回転の最小許容範囲.
	float			m_ForceMax;				//吹き飛ばし量の最大.

	float			m_Gravity;				//重力.
	float			m_RotateSpeed;			//回転する速度.
	float			m_CurrentTiltAngle;		//現在の傾き角度.

	float			m_GroundPos;			//地面の位置(仮).

	D3DXQUATERNION	m_StartQuat;			//開始時の回転.

private:
//======================================================================
// 	   呼び出しを禁止するコンストラクタ.
//======================================================================
	CPlayerFallingState() = delete;	//デフォルトコンストラクタ削除.
	CPlayerFallingState(const CPlayerFallingState&) = delete;
	CPlayerFallingState& operator = (const CPlayerFallingState& rhs) = delete;

};