#pragma once

#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

/**************************************************
*	地面クラス.
**/
class CGround
	: public CStaticMeshObject
{
public:
	CGround();
	~CGround();

	void Update() override;

	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

protected:
	float	FallSpeed;	//落ちる速度.
};