#include "CSky.h"

CSky::CSky()
{
	AttachSprite(AssetManager::Sprite(Sprite2DList::Sky));
}

CSky::~CSky()
{
}

//======================================================================
// 	   äOïîÇ≈åƒÇ—èoÇ∑ä÷êî.
//======================================================================

void CSky::Update()
{
	CUIObject::Update();
}

void CSky::Draw()
{
	CUIObject::Draw();
}