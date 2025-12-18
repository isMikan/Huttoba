#include "CSky.h"

CSky::CSky()
{
}

CSky::~CSky()
{
}

//======================================================================
// 	   äOïîÇ≈åƒÇ—èoÇ∑ä÷êî.
//======================================================================

void CSky::Update()
{
	CSpriteObject::Update();
}

void CSky::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj)
{
	CSpriteObject::Draw(View, Proj);
}