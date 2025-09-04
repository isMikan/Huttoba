#include "stdafx.h"
#include "SpriteManager.h"

SpriteManager::SpriteManager()
	: m_p2DSpritMap()
	, m_p3DSpritMap()
	, m_Sprite2DInfoList()
	, m_Sprite3DInfoList()
{
}

SpriteManager::~SpriteManager()
{
}

//--------------------------------------------------------------------------------------------------------------

HRESULT SpriteManager::Create()
{
	if (CreateSprite2D() != S_OK) { return E_POINTER; }
	if (CreateSprite3D() != S_OK) { return E_POINTER; }
	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

HRESULT SpriteManager::LoadData()
{
	if (LoadSprite2D() != S_OK) { return E_FAIL; }
	if (LoadSprite3D() != S_OK) { return E_FAIL; }
	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

HRESULT SpriteManager::CreateSprite3D()
{
	//äesprite3DÇÃèÓïÒÇìoò^
//Å´-----------------------------------------------------------------------------------------------------------------------------Å´
	RegisterSprite3D(Sprite3DList::Explosion,	_T("Data\\Texture\\explosion.png"), { 1.f, 1.f, 256.f, 256.f, 32.f, 32.f	});
//Å™-----------------------------------------------------------------------------------------------------------------------------Å™

	//ìoò^ÇµÇΩsprite3DÇÃÉCÉìÉXÉ^ÉìÉXçÏê¨.
	for (auto& sprite3D : m_Sprite3DInfoList)
	{
		m_p3DSpritMap[sprite3D.Id] = std::make_unique<CSprite3D>();
		if (!m_p3DSpritMap[sprite3D.Id]) return E_POINTER;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

HRESULT SpriteManager::CreateSprite2D()
{
	//äesprite2DÇÃèÓïÒÇìoò^(Ç±Ç±Ç…sprite2Dìoò^)
//Å´-----------------------------------------------------------------------------------------------------------------------------Å´
	RegisterSprite2D(Sprite2DList::PMon,_T("Data\\Texture\\pmon.png"),{ 64.f, 64.f, 896.f, 560.f, 896.f / 16.f, 560.f / 10.f });
	RegisterSprite2D(Sprite2DList::Title, _T("Data\\Texture\\Title_kari.png"), { WND_W,WND_H,WND_W,WND_H,WND_W,WND_H });
	RegisterSprite2D(Sprite2DList::Standby, _T("Data\\Texture\\Standby_kari.png"), { WND_W,WND_H,WND_W,WND_H,WND_W,WND_H });
	RegisterSprite2D(Sprite2DList::RedyFont, _T("Data\\Texture\\Ready_kari.png"), { 200,100, 200,100, 200,100 });
	RegisterSprite2D(Sprite2DList::NotRedyFont, _T("Data\\Texture\\NotReady_kari.png"), { 200,100, 200,100, 200,100 });
	RegisterSprite2D(Sprite2DList::Result, _T("Data\\Texture\\Result_kari.png"), { WND_W,WND_H,WND_W,WND_H,WND_W,WND_H });
	RegisterSprite2D(Sprite2DList::Selector,_T("Data\\Texture\\yajirusi_kari.png"), { 64,64,64,64,64,64 });
//Å™-----------------------------------------------------------------------------------------------------------------------------Å™

	//äesprite2DÇÃÉCÉìÉXÉ^ÉìÉXçÏê¨.
	for (auto& sprite2D : m_Sprite2DInfoList)
	{
		m_p2DSpritMap[sprite2D.Id] = std::make_unique<CSprite2D>();
		if (!m_p2DSpritMap[sprite2D.Id]) return E_POINTER;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

HRESULT SpriteManager::LoadSprite2D()
{
	//sprite2Dì«Ç›çûÇ›
	for (auto& sprite2D : m_Sprite2DInfoList)
	{
		if(m_p2DSpritMap[sprite2D.Id]->Init(sprite2D.Path, sprite2D.Size) != S_OK)
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

HRESULT SpriteManager::LoadSprite3D()
{
	//sprite3Dì«Ç›çûÇ›
	for (auto& sprite3D : m_Sprite3DInfoList)
	{
		if (m_p3DSpritMap[sprite3D.Id]->Init(sprite3D.Path, sprite3D.Size) != S_OK)
			return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------

std::shared_ptr<CSprite3D> SpriteManager::GetSprite3D(Sprite3DList spriteID)
{
	auto it = m_p3DSpritMap.find(spriteID);
	if (it != m_p3DSpritMap.end())
	{
		return it->second;
	}
	return nullptr;
}

//--------------------------------------------------------------------------------------------------------------

std::shared_ptr<CSprite2D> SpriteManager::GetSprite2D(Sprite2DList spriteID)
{
	auto it = m_p2DSpritMap.find(spriteID);
	if (it != m_p2DSpritMap.end())
	{
		return it->second;
	}
	return nullptr;
}

//--------------------------------------------------------------------------------------------------------------

void SpriteManager::RegisterSprite2D(Sprite2DList spriteID, LPCTSTR path, CSprite2D::SPRITE_STATE size)
{
	m_Sprite2DInfoList.push_back({ spriteID, path, size });
}

//--------------------------------------------------------------------------------------------------------------

void SpriteManager::RegisterSprite3D(Sprite3DList spriteID, LPCTSTR path, CSprite3D::SPRITE_STATE size)
{
	m_Sprite3DInfoList.push_back({ spriteID, path, size });
}