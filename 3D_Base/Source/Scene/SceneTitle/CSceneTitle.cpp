#include "CSceneTitle.h"

CSceneTitle::CSceneTitle()
	: m_pSpriteTitlImg	( nullptr )
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

	if (m_pInput->IsDown(Action::Decide))
	{
		SetNextScene(GameMain);
	}

	//if (abs(m_pInput->GetLeftSthikY()) >= m_SthikThreshold)
	if (GetAsyncKeyState('W') & 0x0001)
	{
		m_SelectorNumber--;
	}
	if (GetAsyncKeyState('S') & 0x0001)
	{
		m_SelectorNumber--;
		
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
	m_SelectorYPos.push_back(500);
}
