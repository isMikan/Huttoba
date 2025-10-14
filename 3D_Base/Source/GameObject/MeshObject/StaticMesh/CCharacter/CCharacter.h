#pragma once

#include "GameObject/MeshObject/StaticMesh/CStaticMeshObject.h"

/**************************************************
*   キャラクタークラス
**/
class CCharacter
	: public CStaticMeshObject
{
public:
	CCharacter();
	virtual ~CCharacter();

	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

protected:
};