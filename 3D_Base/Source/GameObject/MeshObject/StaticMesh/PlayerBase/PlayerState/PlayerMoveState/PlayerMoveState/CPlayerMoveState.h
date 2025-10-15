#pragma once

#include "GameObject/MeshObject/StaticMesh/PlayerBase/PlayerState/CPlayerState.h"

/**************************************************
*	プレイヤーが移動する状態クラス.
**/
class CPlayerMoveState
	: public CPlayerState	//プレイヤー状態継承クラス.
{
public:
	CPlayerMoveState(float x, float z);	//引数:移動方向のx軸、z軸.
	~CPlayerMoveState() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayerBase& pPlayerBase) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayerBase& pPlayerBase) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayerBase& pPlayerBase) override;

private:
	D3DXVECTOR3 m_Dir;				//方向キーの入力.

	float		m_CurrentSpeed;		//現在の速さ.
	float		m_MoveSpeed;		//移動速度.
	float		m_RotatingMoveSpeed;//回転している時の速度.
	float		m_MoveSpeedScale;	//移動量.

	D3DXVECTOR3	m_MoveDir;			//目的の方向.

private:
	CPlayerMoveState() = delete;	//デフォルトコンストラクタ禁止.
	CPlayerMoveState(const CPlayerMoveState&) = delete;
	CPlayerMoveState& operator = (const CPlayerMoveState& rhs) = delete;

};
