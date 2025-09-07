#include "CSceneTitle.h"

CSceneTitle::CSceneTitle(HWND hWnd, CInputManager& inputmanager)
	: m_hWnd			(hWnd)

	, m_Action			()

	, m_pSpriteTitlImg	( nullptr )
	, m_pSpriteSelector	( nullptr )

	, m_pSpriteTitleFont( nullptr )
	, m_pSpriteStartFont( nullptr )
	, m_pSpriteEndFont	( nullptr )
	
	, m_InputManager	( inputmanager )

	, m_SelectorPos		()

	, m_SelectorNumber	( 0 )

	//, m_SthikThreshold	(0.5f)		//ここの値を変えると選択肢を動かす
									//スティックの最低値が変化する.

{
	Create();
	LoadData();
	SetSelectorPos();
}

CSceneTitle::~CSceneTitle()
{
}

HRESULT CSceneTitle::Create()
{
	m_pSpriteTitlImg = std::make_unique<CUIObject>();
	m_pSpriteSelector = std::make_unique<CUIObject>();

	m_pSpriteTitleFont = std::make_unique<CUIObject>();
	m_pSpriteStartFont = std::make_unique<CUIObject>();
	m_pSpriteEndFont = std::make_unique<CUIObject>();

	return S_OK;
}

HRESULT CSceneTitle::LoadData()
{
	m_pSpriteTitlImg->AttachSprite(AssetManager::Sprite(Sprite2DList::Title));

	m_pSpriteSelector->AttachSprite(AssetManager::Sprite(Sprite2DList::Selector));

	m_pSpriteTitleFont->AttachSprite(AssetManager::Sprite(Sprite2DList::BigFontUI));
	m_pSpriteTitleFont->SetPatternNo(0, 0);
	m_pSpriteTitleFont->SetPosition(330, 50, 0);

	m_pSpriteStartFont->AttachSprite(AssetManager::Sprite(Sprite2DList::FontUI));
	m_pSpriteStartFont->SetPatternNo(0, 0);
	m_pSpriteStartFont->SetPosition(550, 440, 0);

	m_pSpriteEndFont->AttachSprite(AssetManager::Sprite(Sprite2DList::FontUI));
	m_pSpriteEndFont->SetPatternNo(0, 1);
	m_pSpriteEndFont->SetPosition(570, 540, 0);

	//関数を入れる
	m_Action =
	{
		//ラムダ式で関数にしてm_Actionの中に入れている(SetNextScene(Standby);ではだめ).
		//画面に表示される選択肢の文字と同じ順番に処理を入れていく
		[this]() {SetNextScene(Standby);},
		[this]() {DestroyWindow(m_hWnd);}
	};

	return S_OK;
}

void CSceneTitle::Update()
{
	m_InputManager.Update();

	MoveSelector();

	if (m_InputManager.GetInput(0).IsDown(Action::Decide))
	{
		//選択中の番号で処理される関数が変わる.
		m_Action[m_SelectorNumber]();
	}
}

void CSceneTitle::Draw()
{
	m_pSpriteTitleFont->Draw();
	m_pSpriteStartFont->Draw();
	m_pSpriteEndFont->Draw();

	m_pSpriteSelector->Draw();

	//m_pSpriteTitlImg->Draw();
}

void CSceneTitle::Destroy()
{
}

void CSceneTitle::SetSelectorPos()
{
	m_SelectorPos.push_back(D3DXVECTOR3(465, 440, 0));
	m_SelectorPos.push_back(D3DXVECTOR3(490, 540, 0));
}

void CSceneTitle::MoveSelector()
{
	if (m_InputManager.GetInput(0).IsDown(Action::NavigateUp))
	{
		if (m_SelectorNumber > 0)
			m_SelectorNumber--;
	}
	if (m_InputManager.GetInput(0).IsDown(Action::NavigateDown))
	{
		if (m_SelectorNumber < m_SelectorPos.size() - 1)
			m_SelectorNumber++;
	}
	m_pSpriteSelector->SetPosition(m_SelectorPos[m_SelectorNumber]);
}
