#pragma once

#include"Sprite2D/CSprite2D.h"
#include"Sprite3D/CSprite3D.h"

class SpriteManager
{
public:

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

private:
	//ゲームで扱うスプライトデータ
	std::unordered_map<Sprite3DList, std::shared_ptr<CSprite3D> > m_p3DSpritMap;

	//スプライト2Dデータ
	std::unordered_map<Sprite2DList, std::shared_ptr<CSprite2D> > m_p2DSpritMap;


};