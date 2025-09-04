#include "Scene/SceneResult/CSceneResult.h"

CSceneResult::CSceneResult(CInputManager& input)
	: m_Action				()

	, m_pSpriteResultImg	( nullptr )
	, m_pSpriteSelector		( nullptr )

	, m_pPlayer				()

	, m_InputManager		( input )

	, m_SelectorPos			()

	, m_SelectorNumber		( 0 )

	, m_SelectorYPos		()
{
	Create();
	LoadData();
	SetSelectorPos();
}

CSceneResult::~CSceneResult()
{
}

HRESULT CSceneResult::Create()
{
	m_pSpriteResultImg = std::make_unique<CUIObject>();
	m_pSpriteSelector = std::make_unique<CUIObject>();

	return S_OK;
}

HRESULT CSceneResult::LoadData()
{
	m_pSpriteResultImg->AttachSprite(AssetManager::Sprite(Sprite2DList::Result));
	m_pSpriteSelector->AttachSprite(AssetManager::Sprite(Sprite2DList::Selector));

	//関数を入れる
	m_Action =
	{
		//ラムダ式で関数にしてm_Actionの中に入れている(SetNextScene(Standby);ではだめ).
		//画面に表示される選択肢の文字と同じ順番に処理を入れていく
		[this]() {SetNextScene(GameMain);},
		[this]() {SetNextScene(Standby);},
		[this]() {SetNextScene(Title);}
	};

	return S_OK;
}


void CSceneResult::Update()
{
	m_InputManager.Update();

	MoveSelector();

	//if (m_InputManager.GetInput(0).IsDown(Action::Decide, true))
	//{
	//	SetNextScene(Title);
	//}

	if (m_InputManager.GetInput(0).IsDown(Action::Decide))
	{
		//選択中の番号で処理される関数が変わる.
		m_Action[m_SelectorNumber]();
	}
}


void CSceneResult::Draw()
{
	m_pSpriteSelector->Draw();
	m_pSpriteResultImg->Draw();
}

void CSceneResult::Destroy()
{
}

void CSceneResult::SetSelectorPos()
{
	m_SelectorPos = D3DXVECTOR3(900, 0, 0);

	m_SelectorYPos.push_back(D3DXVECTOR3(900, 410, 0));
	m_SelectorYPos.push_back(D3DXVECTOR3(850, 510, 0));
	m_SelectorYPos.push_back(D3DXVECTOR3(900, 590, 0));
}

void CSceneResult::MoveSelector()
{
	if (m_InputManager.GetInput(0).IsDown(Action::NavigateUp))
	{
		if (m_SelectorNumber > 0)
			m_SelectorNumber--;
	}
	if (m_InputManager.GetInput(0).IsDown(Action::NavigateDown))
	{
		if (m_SelectorNumber < m_SelectorYPos.size() - 1)
			m_SelectorNumber++;
	}

	//m_SelectorPos.y = m_SelectorYPos[m_SelectorNumber];
	m_pSpriteSelector->SetPosition(m_SelectorYPos[m_SelectorNumber]);
}