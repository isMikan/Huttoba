#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerState/DirectionalInputState/CPlayerDirectionalInputState.h"

class CPlayerRotation
	: public CPlayerDirectionalInputState
{
public:
	CPlayerRotation();
	~CPlayerRotation() override;

	//状態が始めるときだけ呼び出される.
	void Enter(CPlayer& pPlayer) override;
	//状態が終わるときだけ呼び出される.
	void Exit(CPlayer& pPlayer) override;
	//この状態の間は毎フレーム呼び出される.
	void Update(CPlayer& pPlayer) override;

	//テスト用のキー処理.
	void KeyInput(CPlayer& pPlayer, float x, float z) override;

private:
	float			m_RotationSpeed;	//回転速度.
	float			m_RotationDir;		//目的の方向.
};