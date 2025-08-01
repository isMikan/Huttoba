#pragma once
#include "Scene/CScene.h"

#include "Sprite/Sprite2D/CSprite2D.h"
#include "CInput/CInput.h"

class CSceneStandby
	: public CScene
{
public:
	CSceneStandby(CInput& input);
	~CSceneStandby() override;

	HRESULT Create() override;
	HRESULT LoadData()	override;
	void Update() override;
	void Draw() override;
	void Destroy() override;

private:

private:
	std::unique_ptr<CSprite2D>	m_pSpriteStandbyImg;	//‘Ò‹@‰æ–Ê.

	CInput&		m_Input;
};
