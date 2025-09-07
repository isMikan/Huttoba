#include "CSceneStandby.h"

CSceneStandby::CSceneStandby(CInputManager& input)
	: m_pSpriteStandbyImg	( nullptr )

	, m_pRedyFontImg		( )
	, m_pNotRedyFontImg		()

	, m_pSpriteSelector		(nullptr)

	, m_pCamera				( nullptr )

	, m_pPlayer				()

	, m_InputManager		( input )

	, m_Action				()

	, m_SelectorPos			()

	, m_SelectorNumber		(0)
{
	Create();
	LoadData();
	//InitializePlayers();
	InitializeRedyFont();
	SetSelectorPos();

	m_pCamera->SetPosition(0.f, 10.f, 10.f);
	m_pCamera->SetLook(0.f, 0.f, -10.f);
	m_pCamera->SetLight(1.5f, 1.f, -1.f);
}

CSceneStandby::~CSceneStandby()
{
}

HRESULT CSceneStandby::Create()
{
	m_pSpriteStandbyImg = std::make_unique<CUIObject>();

	for (int i = 0;i < 4;i++)
	{
		m_pRedyFontImg[i] = std::make_unique<CUIObject>();
		m_pNotRedyFontImg[i] = std::make_unique<CUIObject>();
	}

	m_pSpriteSelector = std::make_unique<CUIObject>();

	m_pCamera = std::make_unique<CCamera>();

	return S_OK;
}

HRESULT CSceneStandby::LoadData()
{
	m_pSpriteStandbyImg->AttachSprite(AssetManager::Sprite(Sprite2DList::Standby));

	for (int i = 0;i < 4;i++)
	{
		m_pRedyFontImg[i]->AttachSprite(AssetManager::Sprite(Sprite2DList::RedyFont));
		m_pNotRedyFontImg[i]->AttachSprite(AssetManager::Sprite(Sprite2DList::NotRedyFont));
	}

	m_pSpriteSelector->AttachSprite(AssetManager::Sprite(Sprite2DList::Selector));


	//関数を入れる
	m_Action =
	{
		//ラムダ式で関数にしてm_Actionの中に入れている(SetNextScene(Standby);ではだめ).
		//画面に表示される選択肢の文字と同じ順番に処理を入れていく
		[this]() {SetNextScene(GameMain);},
		[this]() {SetNextScene(Title);}
	};

	return S_OK;
}

void CSceneStandby::Update()
{
	m_InputManager.Update();

	MoveSelector();

	auto& input = m_InputManager.GetInput(0);

	// コントローラー0は「決定可能」
	auto& input0 = m_InputManager.GetInput(0);
	auto& slot0 = m_InputManager.GetSlot(0);
	if (input0.IsDown(Action::Decide)/* && slot0.ready*/)
	{
		//SetNextScene(GameMain);
		//選択中の番号で処理される関数が変わる.
		m_Action[m_SelectorNumber]();
	}

	//コントローラーで準備状態切り替え.
	for (int i = 0; i < 4; ++i)
	{
		auto& input = m_InputManager.GetInput(i);

		if (input.IsDown(Action::Switch))
		{
			m_InputManager.ChangeSlot(i);
		}
	}
}

void CSceneStandby::Draw()
{
	m_pCamera->Update();	//値をとる前に処理したいのでここに入れた(Updateに入れるべきかも.

	D3DXMATRIX	mView = m_pCamera->GetView();
	D3DXMATRIX	mProj = m_pCamera->GetProj();
	LIGHT		light = m_pCamera->GetLight();
	CAMERA		camera = m_pCamera->GetCamera();

	for (int i = 0;i < 4;i++)
	{
		if (m_InputManager.GetSlot(i).ready)
		{
			m_pRedyFontImg[i]->Draw();
		}
		else
		{
			m_pNotRedyFontImg[i]->Draw();
		}
	}

	m_pSpriteSelector->Draw();

	m_pSpriteStandbyImg->Draw();	//一番前に表示されるので文字などを表示させたい際は要検証.

}

void CSceneStandby::Destroy()
{
}

//void CSceneStandby::InitializePlayers()
//{
//	////プレイヤーの人数だけ処理(マジックナンバーなのを後で変える).
//	//for (int i = 0;i < 4;i++)
//	//{
//	//	m_pPlayer.push_back(std::make_unique<CPlayer>());
//	//	m_pPlayer[i]->AttachMesh(AssetManager::Mesh(StaticMeshList::Player));
//	//	m_pPlayer[i]->SetPosition(-12.f + (9.f * i), 1.f, 6.f);
//	//}
//}

void CSceneStandby::InitializeRedyFont()
{
	for (int i = 0;i < 4;i++)
	{
		m_pRedyFontImg[i]->SetPosition(200 + (230 * i), 240, 0);
		m_pNotRedyFontImg[i]->SetPosition(200 + (230 * i), 240, 0);
	}
}

void CSceneStandby::SetSelectorPos()
{
	m_SelectorPos.push_back(D3DXVECTOR3(455, 450, 0));
	m_SelectorPos.push_back(D3DXVECTOR3(440, 550, 0));
}

void CSceneStandby::MoveSelector()
{
	if (m_InputManager.GetInput(0).IsDown(Action::NavigateUp) || 0 < m_InputManager.GetInput(0).GetLeftSthikY())
	{
		if (m_SelectorNumber > 0)
			m_SelectorNumber--;
	}
	if (m_InputManager.GetInput(0).IsDown(Action::NavigateDown) || 0 > m_InputManager.GetInput(0).GetLeftSthikY())
	{
		if (m_SelectorNumber < m_SelectorPos.size() - 1)
			m_SelectorNumber++;
	}

	m_pSpriteSelector->SetPosition(m_SelectorPos[m_SelectorNumber]);
}
