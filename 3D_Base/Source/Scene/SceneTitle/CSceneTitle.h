#pragma once
#include "Scene/CScene.h"

#include "Sprite/Sprite2D/CSprite2D.h"
#include "CInput/CInput.h"

class CSceneTitle
	: public CScene
{
public:
	CSceneTitle(HWND hWnd, CInput& input);
	~CSceneTitle() override;

	HRESULT Create() override;
	HRESULT LoadData()	override;
	void Update() override;
	void Draw() override;
	void Destroy() override;

private:
	//選択肢の座標を設定.
	void SetSelectorPos();

private:
	HWND						m_hWnd;				//ウィンドウハンドル.

	std::unique_ptr<CSprite2D>	m_pSpriteTitlImg;	//タイトル画面.
	std::unique_ptr<CSprite2D>	m_pSpriteSelector;	//選択矢印.

	CInput&						m_Input;

	D3DXVECTOR3					m_SelectorPos;		//選択矢印の座標.

	//8ビットの符号付整数型.
	uint8_t						m_SelectorNumber;	//現在選択中の番号.

	std::vector<float>			m_SelectorYPos;		//各選択肢のY座標.

	float						m_SthikThreshold;	//選択肢を動かすときのスティックの最小値.
};
