#pragma once

#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

class DrawCollision
	: public CStaticMeshObject
{
public:
	DrawCollision();
	~DrawCollision();

	HRESULT LoadData();
	void Init();
	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;


private:

};