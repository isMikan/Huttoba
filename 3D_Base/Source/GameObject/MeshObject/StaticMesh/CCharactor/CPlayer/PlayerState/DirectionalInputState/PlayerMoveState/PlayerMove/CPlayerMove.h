#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/DirectionalInputState/CPlayerDirectionalInputState.h"

class CPlayerMove
	: public CPlayerDirectionalInputState
{
public:
	CPlayerMove();
	~CPlayerMove() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;

	//スティックかカーソル入力を感知.
	void KeyInput(CPlayer& pPlayer, float x, float z) override;

	std::string GetStateName() const override;


private:
	float		m_CurrentSpeed;		//現在の速さ.
	float		m_MoveSpeed;		//移動速度.
	float		m_RotatingMoveSpeed;//回転している時の速度.
	float		m_MoveSpeedScale;	//移動量.

	D3DXVECTOR3	m_MoveDir;			//目的の方向.
};
