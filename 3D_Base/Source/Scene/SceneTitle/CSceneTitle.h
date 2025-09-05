#pragma once
#include "Scene/CScene.h"

#include "GameObject/UIObject/CUIObject.h"
#include "Assets/AssetManager.h"
#include "CInput/CInputManager.h"

class CSceneTitle
	: public CScene
{
public:
	CSceneTitle(HWND hWnd, CInputManager& input);
	~CSceneTitle() override;

	HRESULT Create() override;
	HRESULT LoadData()	override;
	void Update() override;
	void Draw() override;
	void Destroy() override;

private:
	//選択肢の座標を設定.
	void SetSelectorPos();

	void MoveSelector();

private:
	HWND						m_hWnd;				//ウィンドウハンドル.

	std::vector<std::function<void()>>	m_Action;	//画面遷移時の動作を入れる.

	std::unique_ptr<CUIObject>	m_pSpriteTitlImg;	//タイトル画面.
	std::unique_ptr<CUIObject>	m_pSpriteSelector;	//選択矢印.

	std::unique_ptr<CUIObject>	m_pSpriteTitleFont;
	std::unique_ptr<CUIObject>	m_pSpriteStartFont;
	std::unique_ptr<CUIObject>	m_pSpriteEndFont;

	CInputManager&				m_InputManager;

	D3DXVECTOR3					m_SelectorPos;		//選択矢印の座標.

	//8ビットの符号付整数型.
	uint8_t						m_SelectorNumber;	//現在選択中の番号.

	std::vector<float>			m_SelectorYPos;		//各選択肢のY座標.

	//中間までに導入する時間がないので時間があれば追加できるようにする
	//float						m_SthikThreshold;	//選択肢を動かすときのスティックの最小値.
};
