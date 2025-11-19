#pragma once

#include "CStaticMeshObject.h"
#include "Ground/GroundManager/CGroundManager.h"

class GroundStatusReader
	: public CStaticMeshObject
	, public ICollisionListener
{
public:
	GroundStatusReader();
	~GroundStatusReader();

private:

};