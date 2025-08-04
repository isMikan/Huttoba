#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CPlayer/PlayerHand/CPlayerHand.h"

/**************************************************
*	プレイヤーの右手クラス.
**/
class CPlayerRightHand
	: public CPlayerHand
{
public:
	CPlayerRightHand();
	~CPlayerRightHand();

	void Update() override;
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

private:
};