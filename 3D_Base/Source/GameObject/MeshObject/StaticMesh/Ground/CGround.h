#pragma once

#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

/*********************************************************
*	’n–ÊƒNƒ‰ƒX
**/
class CGround
	: public CStaticMeshObject
{
public:
	CGround();
	virtual ~CGround();

	virtual void Update() override;

	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj,
		LIGHT& Light, CAMERA& Camera) override;

protected:

};