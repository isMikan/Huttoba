#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerHand/CPlayerHand.h"

/**************************************************
*	プレイヤーの右手クラス.
**/
class CPlayerLeftHand
	: public CPlayerHand
{
public:
	CPlayerLeftHand();
	~CPlayerLeftHand();

	void Update() override;
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

private:
};