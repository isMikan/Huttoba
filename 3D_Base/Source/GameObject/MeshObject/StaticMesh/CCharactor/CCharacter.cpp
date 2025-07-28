#include "CCharacter.h"

CCharacter::CCharacter()
{
}

CCharacter::~CCharacter()
{
}

void CCharacter::Update()
{
	CStaticMeshObject::Update();
}

void CCharacter::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}
