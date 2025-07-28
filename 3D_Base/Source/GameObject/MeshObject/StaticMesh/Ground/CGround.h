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

protected:

};