#pragma once

#include"Sprite2D/CSprite2D.h"
#include"Sprite3D/CSprite3D.h"

class SpriteManager
{
public:

	//外部から情報を取得するための前設計。まだ使いにくいかもだけど許してください。
	struct Sprite2DMeshInfo
	{
		Sprite2DList Id;
		LPCTSTR Path;
		CSprite2D::SPRITE_STATE Size;
	};

	struct Sprite3DMeshInfo
	{
		Sprite3DList Id;
		LPCTSTR Path;
		CSprite3D::SPRITE_STATE Size;
	};


	SpriteManager();
	~SpriteManager();

	HRESULT Create();
	HRESULT LoadData();

	std::shared_ptr<CSprite3D> GetSprite3D(Sprite3DList spriteID);
	std::shared_ptr<CSprite2D> GetSprite2D(Sprite2DList spriteID);

private:

	HRESULT CreateSprite3D();
	HRESULT CreateSprite2D();

	HRESULT LoadSprite2D();
	HRESULT LoadSprite3D();

	void RegisterSprite2D(Sprite2DList spriteID, LPCTSTR path, CSprite2D::SPRITE_STATE size);
	void RegisterSprite3D(Sprite3DList spriteID, LPCTSTR path, CSprite3D::SPRITE_STATE size);

private:
	//ゲームで扱うスプライトデータ
	std::unordered_map<Sprite3DList, std::shared_ptr<CSprite3D> > m_p3DSpritMap;

	//スプライト2Dデータ
	std::unordered_map<Sprite2DList, std::shared_ptr<CSprite2D> > m_p2DSpritMap;

	std::vector<Sprite2DMeshInfo> m_Sprite2DMeshInfoList;
	std::vector<Sprite3DMeshInfo> m_Sprite3DMeshInfoList;
};