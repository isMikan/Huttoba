#pragma once

#include "GameObject/MeshObject/StaticMesh/CCharactor/CCharacter.h"

/**************************************************
*	エネミークラス
**/
class CEnemy
	: public CCharacter
{
public:
	CEnemy();
	virtual ~CEnemy();

	virtual void Update() override;

protected:

};
