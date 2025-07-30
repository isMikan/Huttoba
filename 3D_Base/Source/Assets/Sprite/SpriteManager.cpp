#include "stdafx.h"
#include "SpriteManager.h"

SpriteManager::SpriteManager()
	: m_p2DSpritMap()
	, m_p3DSpritMap()
{
}

SpriteManager::~SpriteManager()
{
}

HRESULT SpriteManager::Create()
{
	if (CreateSprite2D() != S_OK) { return E_FAIL; }
	if (CreateSprite3D() != S_OK) { return E_FAIL; }
}

HRESULT SpriteManager::LoadData()
{
	if (LoadSprite2D() != S_OK) { return E_FAIL; }
	if (LoadSprite3D() != S_OK) { return E_FAIL; }
}

HRESULT SpriteManager::CreateSprite3D()
{
	//スプライトのインスタンス作成.
	Sprite3DList spritw3DList[] =
	{
		Sprite3DList::Ground,
		Sprite3DList::Player,
		Sprite3DList::Explosion,
	};

	for (auto& id : spritw3DList)
	{
		m_p3DSpritMap[id] = std::make_unique<CSprite3D>();
		if (!m_p3DSpritMap[id]) return E_POINTER;
	}

	return S_OK;
}

HRESULT SpriteManager::CreateSprite2D()
{
	Sprite2DList sprite2DList[] =
	{
		Sprite2DList::PMon
	};

	for (auto& id : sprite2DList)
	{
		m_p2DSpritMap[id] = std::make_unique<CSprite2D>();
		if (!m_p2DSpritMap[id]) return E_POINTER;

	}

	return S_OK;
}

HRESULT SpriteManager::LoadSprite2D()
{

	//Pモンスプライトの構造体
	CSprite2D::SPRITE_STATE SSPmon =
	{ 64.f, 64.f, 896.f, 560.f, 896.f / 16.f, 560.f / 10.f };

	//Pモンスプライトの読み込み
	m_p2DSpritMap[Sprite2DList::PMon]->Init(_T("Data\\Texture\\pmon.png"), SSPmon);

	return S_OK;
}

HRESULT SpriteManager::LoadSprite3D()
{
	//地面スプライトの構造体
	CSprite3D::SPRITE_STATE SSGround;
	SSGround.Disp.w = 1.f;
	SSGround.Disp.h = 1.f;
	SSGround.Base.w = 256.f;
	SSGround.Base.h = 256.f;
	SSGround.Stride.w = 256.f;
	SSGround.Stride.h = 256.f;

	//地面スプライトの読み込み.
	m_p3DSpritMap[Sprite3DList::Ground]->Init(_T("Data\\Texture\\Ground.png"), SSGround);

	//プレイヤースプライトの構造体
	CSprite3D::SPRITE_STATE SSPlayer =
	{ 1.f, 1.f, 64.f, 64.f, 64.f, 64.f };

	//プレイヤースプライトの読み込み.
	m_p3DSpritMap[Sprite3DList::Player]->Init(_T("Data\\Texture\\Player.png"), SSPlayer);

	//爆発スプライトの構造体
	CSprite3D::SPRITE_STATE SSExplosion =
	{ 1.f, 1.f, 256.f, 256.f, 32.f, 32.f };

	//爆発スプライトの読み込み.
	m_p3DSpritMap[Sprite3DList::Explosion]->Init(_T("Data\\Texture\\explosion.png"), SSExplosion);

	return S_OK;
}

std::shared_ptr<CSprite3D> SpriteManager::GetSprite3D(Sprite3DList spriteID)
{
	auto it = m_p3DSpritMap.find(spriteID);
	if (it != m_p3DSpritMap.end())
	{
		return it->second;
	}
	return nullptr;
}

std::shared_ptr<CSprite2D> SpriteManager::GetSprite2D(Sprite2DList spriteID)
{
	auto it = m_p2DSpritMap.find(spriteID);
	if (it != m_p2DSpritMap.end())
	{
		return it->second;
	}
	return nullptr;
}