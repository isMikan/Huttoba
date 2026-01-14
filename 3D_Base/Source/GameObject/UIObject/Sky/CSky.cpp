#include "CSky.h"

CSky::CSky()
	: m_MoveUV		( 0.f, 0.f )
{
	AttachSprite(AssetManager::Sprite(Sprite2DList::Sky));
}

CSky::~CSky()
{
}

//======================================================================
// 	   ŠO•”‚ÅŒÄ‚Ño‚·ŠÖ”.
//======================================================================

void CSky::Update()
{
	CUIObject::Update();
}

void CSky::Draw()
{
	m_MoveUV.x += 0.001f;
	
	m_pSprite->SetUVInfo(m_MoveUV);
	m_pSprite->SetIsMoving(true);

	CUIObject::Draw();
}