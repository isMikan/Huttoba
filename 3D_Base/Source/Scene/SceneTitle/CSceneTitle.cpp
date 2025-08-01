#include "CSceneTitle.h"

CSceneTitle::CSceneTitle(HWND hWnd)
	: m_hWnd			(hWnd)
	, m_pSpriteTitlImg	( nullptr )
	, m_pSpriteSelector	( nullptr )
	
	, m_pInput			( nullptr )

	, m_SelectorPos		()

	, m_SelectorNumber	( 0 )

	, m_SelectorYPos	()

	, m_SthikThreshold	(0.5f)		//ここの値を変えると選択肢を動かす
									//スティックの最低値が変化する.

{
	Create();
	LoadData();
	SetInputBInding();
	SetSelectorPos();
}

CSceneTitle::~CSceneTitle()
{
}

HRESULT CSceneTitle::Create()
{
	m_pSpriteTitlImg = std::make_unique<CSprite2D>();
	m_pSpriteSelector = std::make_unique<CSprite2D>();

	m_pInput = std::make_unique<CInput>(0);

	return S_OK;
}

HRESULT CSceneTitle::LoadData()
{
	CSprite2D::SPRITE_STATE Title =
	{ WND_W,WND_H,WND_W,WND_H,WND_W,WND_H };

	m_pSpriteTitlImg->Init(_T("Data\\Texture\\Title_kari.png"), Title);

	CSprite2D::SPRITE_STATE Selector =
	{ 64,64,64,64,64,64 };

	m_pSpriteSelector->Init(_T("Data\\Texture\\yajirusi_kari.png"), Selector);

	return S_OK;
}

void CSceneTitle::Update()
{
	m_pInput->Update();

	if (m_pInput->IsDown(Action::NavigateUp))
	{
		if (m_SelectorNumber > 0)
			m_SelectorNumber--;
	}
	if (m_pInput->IsDown(Action::NavigateDown))
	{
		if (m_SelectorNumber < m_SelectorYPos.size() - 1)
			m_SelectorNumber++;
	}

	m_pSpriteSelector->SetPositionY(m_SelectorYPos[m_SelectorNumber]);

	if (m_pInput->IsDown(Action::Decide))
	{
		switch (m_SelectorNumber)
		{
		case 0:
			SetNextScene(GameMain);
			break;
		case 1:
			DestroyWindow(m_hWnd);
			break;
		default:
			break;
		}
	}
}

void CSceneTitle::Draw()
{
	m_pSpriteSelector->Render();
	m_pSpriteTitlImg->Render();
}

void CSceneTitle::Destroy()
{
}

void CSceneTitle::SetInputBInding()
{
	m_pInput->BindKey(Action::Decide, InputBinding(InputDevice::GamePad, CXInput::B));
	m_pInput->BindKey(Action::Decide, InputBinding(InputDevice::Keyboard, 'Z'));


	m_pInput->BindKey(Action::NavigateUp, InputBinding(InputDevice::GamePad, CXInput::UP));
	m_pInput->BindKey(Action::NavigateUp, InputBinding(InputDevice::Keyboard, VK_UP));

	m_pInput->BindKey(Action::NavigateDown, InputBinding(InputDevice::GamePad, CXInput::DOWN));
	m_pInput->BindKey(Action::NavigateDown, InputBinding(InputDevice::Keyboard, VK_DOWN));
}

void CSceneTitle::SetSelectorPos()
{
	m_SelectorPos = D3DXVECTOR3(500, 0, 0);
	m_pSpriteSelector->SetPosition(m_SelectorPos);

	m_SelectorYPos.push_back(430);
	m_SelectorYPos.push_back(540);
}
