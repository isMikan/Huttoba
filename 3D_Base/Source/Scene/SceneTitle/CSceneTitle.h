#pragma once
#include "Scene/CScene.h"

#include "Sprite/Sprite2D/CSprite2D.h"
#include "CInput/CInput.h"

class CSceneTitle
	: public CScene
{
public:
	CSceneTitle();
	~CSceneTitle() override;

	HRESULT Create() override;
	HRESULT LoadData()	override;
	void Update() override;
	void Draw() override;
	void Destroy() override;

	//入力情報の設定.
	void SetInputBInding();

private:
	std::unique_ptr<CSprite2D>	m_pSpriteTitlImg;	//タイトル画面.
	std::unique_ptr<CSprite2D>	m_pSpriteSelector;	//選択矢印.

	std::unique_ptr<CInput>		m_pInput;

	bool						m_IsSelector;		//どちらを選択しているかを判定.

	float						m_SpriteSelectorYPos;

	float						m_SthikThreshold;	//選択肢を動かすときのスティックの最小値.
};
