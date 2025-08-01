#include "stdafx.h"
#include "SpriteManager.h"

SpriteManager::SpriteManager()
	: m_p2DSpritMap()
	, m_p3DSpritMap()
	, m_Sprite2DMeshInfoList()
	, m_Sprite3DMeshInfoList()
{
}

SpriteManager::~SpriteManager()
{
}

HRESULT SpriteManager::Create()
{
	if (CreateSprite2D() != S_OK) { return E_FAIL; }
	if (CreateSprite3D() != S_OK) { return E_FAIL; }
	return S_OK;
}

HRESULT SpriteManager::LoadData()
{
	if (LoadSprite2D() != S_OK) { return E_FAIL; }
	if (LoadSprite3D() != S_OK) { return E_FAIL; }
	return S_OK;
}

HRESULT SpriteManager::CreateSprite3D()
{
	//各3Dスプライトの情報を登録(ここに3Dスプライト登録)
//↓-----------------------------------------------------------------------------------------------------------------------------↓
	RegisterSprite3D(Sprite3DList::Ground,		_T("Data\\Texture\\Ground.png"),	{ 1.f, 1.f, 256.f, 256.f, 256.f, 256.f	});
	RegisterSprite3D(Sprite3DList::Player,		_T("Data\\Texture\\Player.png"),	{ 1.f, 1.f, 64.f, 64.f, 64.f, 64.f		});
	RegisterSprite3D(Sprite3DList::Explosion,	_T("Data\\Texture\\explosion.png"), { 1.f, 1.f, 256.f, 256.f, 32.f, 32.f	});
//↑-----------------------------------------------------------------------------------------------------------------------------↑

	for (auto& sprite3D : m_Sprite3DMeshInfoList)
	{
		m_p3DSpritMap[sprite3D.Id] = std::make_unique<CSprite3D>();
		if (!m_p3DSpritMap[sprite3D.Id]) return E_POINTER;
	}

	return S_OK;
}

HRESULT SpriteManager::CreateSprite2D()
{
	//各2Dスプライトの情報を登録(ここに2Dスプライト登録)
//↓-----------------------------------------------------------------------------------------------------------------------------↓
	RegisterSprite2D(Sprite2DList::PMon,_T("Data\\Texture\\pmon.png"),{ 64.f, 64.f, 896.f, 560.f, 896.f / 16.f, 560.f / 10.f });
//↑-----------------------------------------------------------------------------------------------------------------------------↑

	//各2Dスプライトのインスタンス作成.
	for (auto& sprite2D : m_Sprite2DMeshInfoList)
	{
		m_p2DSpritMap[sprite2D.Id] = std::make_unique<CSprite2D>();
		if (!m_p2DSpritMap[sprite2D.Id]) return E_POINTER;
	}

	return S_OK;
}

HRESULT SpriteManager::LoadSprite2D()
{
	//2Dスプライト読み込み
	for (auto& sprite2D : m_Sprite2DMeshInfoList)
	{
		if(m_p2DSpritMap[sprite2D.Id]->Init(sprite2D.Path, sprite2D.Size) != S_OK)
		return E_FAIL;
	}

	return S_OK;
}

HRESULT SpriteManager::LoadSprite3D()
{
	//3Dスプライト読み込み
	for (auto& sprite3D : m_Sprite3DMeshInfoList)
	{
		if (m_p3DSpritMap[sprite3D.Id]->Init(sprite3D.Path, sprite3D.Size) != S_OK)
			return E_FAIL;
	}

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

void SpriteManager::RegisterSprite2D(Sprite2DList spriteID, LPCTSTR path, CSprite2D::SPRITE_STATE size)
{
	m_Sprite2DMeshInfoList.push_back({ spriteID, path, size });
}

void SpriteManager::RegisterSprite3D(Sprite3DList spriteID, LPCTSTR path, CSprite3D::SPRITE_STATE size)
{
	m_Sprite3DMeshInfoList.push_back({ spriteID, path, size });
}
