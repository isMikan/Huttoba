#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/********************************************************************************************
*	プレイヤーが移動する状態クラス.
**/
class CPlayerMoveState
	: public CPlayerState	//プレイヤー状態を継承.
{
public:
//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	CPlayerMoveState(CPlayerBase& pPlayer, float x, float z);	//引数:移動方向のx軸、z軸.
	~CPlayerMoveState() override;

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
	//--- 現在の速度にふさわしい数値を渡す ---.
	float GetMoveSpeed();

private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	D3DXVECTOR3 m_InputDir;			//入力を受けた方向.

	float		m_CurrentSpeed;		//現在の速さ.
	float		m_MoveSpeed;		//移動速度.
	float		m_RotatingMoveSpeed;//回転している時の速度.
	float		m_HitingMoveSpeed;	//攻撃を受けている時の速度.


private:
//======================================================================
// 	   使用を禁止する関数.
//======================================================================
	CPlayerMoveState() = delete;	//デフォルトコンストラクタ削除.
	CPlayerMoveState(const CPlayerMoveState&) = delete;
	CPlayerMoveState& operator = (const CPlayerMoveState& rhs) = delete;

};
