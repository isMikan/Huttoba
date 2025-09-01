#include "CSceneTitle.h"

CSceneTitle::CSceneTitle(HWND hWnd, CInput& input)
	: m_hWnd			(hWnd)

	, m_Action			()

	, m_pSpriteTitlImg	( nullptr )
	, m_pSpriteSelector	( nullptr )
	
	, m_Input			( input )

	, m_SelectorPos		()

	, m_SelectorNumber	( 0 )

	, m_SelectorYPos	()


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

	return S_OK;
}

HRESULT CSceneTitle::LoadData()
{
	m_pSpriteTitlImg->AttachSprite(AssetManager::Sprite(Sprite2DList::Title));

	m_pSpriteSelector->AttachSprite(AssetManager::Sprite(Sprite2DList::Selector));

	return S_OK;
}

void CSceneTitle::Update()
{
	m_Input.Update();

	MoveSelector();

	//関数を入れる
	m_Action =
	{
		//ラムダ式で関数にしてm_Actionの中に入れている(SetNextScene(Standby);ではだめ).
		//画面に表示される選択肢の文字と同じ順番に処理を入れていく
		[this]() {SetNextScene(Standby);},
		[this]() {DestroyWindow(m_hWnd);}
	};

	if (m_Input.IsDown(Action::Decide))
	{
		//選択中の番号で処理される関数が変わる.
		m_Action[m_SelectorNumber]();
	}
}

void CSceneTitle::Draw()
{
	m_pSpriteSelector->Draw();
	m_pSpriteTitlImg->Draw();
}

void CSceneTitle::Destroy()
{
}

void CSceneTitle::SetSelectorPos()
{
	m_SelectorPos = D3DXVECTOR3(500, 0, 0);

	m_SelectorYPos.push_back(430);
	m_SelectorYPos.push_back(540);
}

void CSceneTitle::MoveSelector()
{
	if (m_Input.IsDown(Action::NavigateUp))
	{
		if (m_SelectorNumber > 0)
			m_SelectorNumber--;
	}
	if (m_Input.IsDown(Action::NavigateDown))
	{
		if (m_SelectorNumber < m_SelectorYPos.size() - 1)
			m_SelectorNumber++;
	}

	m_SelectorPos.y = m_SelectorYPos[m_SelectorNumber];
	m_pSpriteSelector->SetPosition(m_SelectorPos);
}
