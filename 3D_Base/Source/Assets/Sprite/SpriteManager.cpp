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
	RegisterSprite3D(Sprite3DList::Shadow,		_T("Data\\Texture\\shadow.png"),	{ 60.f, 60.f, 60.f, 60.f, 60.f, 60.f });
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
	RegisterSprite2D(Sprite2DList::Fade,			_T("Data\\Texture\\fade.png"),				{ WND_W,WND_H,WND_W,WND_H,WND_W,WND_H });
	RegisterSprite2D(Sprite2DList::Sky,				_T("Data\\Texture\\sky.png"),				{ WND_W,WND_H,WND_W,WND_H,WND_W,WND_H });
	RegisterSprite2D(Sprite2DList::GaugeFrame,		_T("Data\\Texture\\gauge_frame.png"),		{ 160.f, 24.f, 160.f, 24.f, 160.f, 24.f });
	RegisterSprite2D(Sprite2DList::Gauge,			_T("Data\\Texture\\gauge.png"),				{ 160.f, 24.f, 160.f, 24.f, 160.f, 24.f });
	RegisterSprite2D(Sprite2DList::Font_Title,		_T("Data\\Texture\\Font_Title.png"),		{ 695,400,695,400,695,400 });
	RegisterSprite2D(Sprite2DList::Font_Result,		_T("Data\\Texture\\Font_WinnerDraw.png"),	{ 608,330,608,660,608,330 });
	RegisterSprite2D(Sprite2DList::Font_ReadyGo,	_T("Data\\Texture\\Font_Readygo_ver2.png"),	{ 693,217, 693,434, 693,217 });
	RegisterSprite2D(Sprite2DList::Font_Finish,		_T("Data\\Texture\\Font_Finish.png"),		{ 970,195, 970,195, 970,195 });
	RegisterSprite2D(Sprite2DList::Font_UI_Common,	_T("Data\\Texture\\Font_UI_Common.png"),	{ 400,64,400,544,400,64 });
	RegisterSprite2D(Sprite2DList::Font_UI_Ready,	_T("Data\\Texture\\Font_UI_Ready.png"),		{ 239,64.5,239,517,239,64.5 });
	RegisterSprite2D(Sprite2DList::UI_Selector,		_T("Data\\Texture\\UI_SelectArrow.png"),	{ 64,64,64,256,64,64 });
	RegisterSprite2D(Sprite2DList::Clock,			_T("Data\\Texture\\clock.png"),				{ 56,63,56,63,56,63 });
	RegisterSprite2D(Sprite2DList::TimeNumber,		_T("Data\\Texture\\number.png"),			{ 40,40,330,32,33,31 });

	
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