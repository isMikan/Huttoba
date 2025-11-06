#include "CSceneStandby.h"

#include "Camera/CameraManager/CCameraManager.h"

CSceneStandby::CSceneStandby()
	: m_pSpriteStandbyImg	( nullptr )

	, m_pRedyFontImg		()
	, m_pNotRedyFontImg		()

	, m_pSpriteSelector		( nullptr )

	, m_pCamera				( nullptr )

	, m_Action				()

	, m_SelectorPos			()

	, m_SelectorNumber		(0)
{
	Create();
	LoadData();
	//InitializePlayers();
	InitializeRedyFont();
	SetSelectorPos();

	CCameraManager::SetPosition(0.f, 10.f, 10.f);
	CCameraManager::SetLook(0.f, 0.f, -10.f);
	CCameraManager::SetLight(1.5f, 1.f, -1.f);
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
	MoveSelector();

	if (CInputManager::IsDown(Action::Decide,0)/* && slot0.ready*/)
	{
		//SetNextScene(GameMain);
		//選択中の番号で処理される関数が変わる.
		m_Action[m_SelectorNumber]();
	}

	//コントローラーで準備状態切り替え.
	for (int i = 0; i < 4; ++i)
	{
		if (CInputManager::IsDown(Action::Switch,i))
		{
			CInputManager::ChangeSlot(i);
		}
	}
}

void CSceneStandby::Draw()
{
	//カメラの処理.
	CCameraManager::Update();

	//=== 情報を取得 ===.
	CAMERA camera = CCameraManager::GetCamera();		//カメラ.
	LIGHT light = CCameraManager::GetLight();			//ライト.
	D3DXMATRIX view = CCameraManager::GetView();		//ビュー.
	D3DXMATRIX proj = CCameraManager::GetProjection();	//プロジェクション.
	//==================.

	for (int i = 0;i < 4;i++)
	{
		if (CInputManager::GetSlot(i).ready)
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

void CSceneStandby::InitializeRedyFont()
{
	for (int i = 0;i < 4;i++)
	{
		m_pRedyFontImg[i]->SetPosition(200 + static_cast<float>(230 * i), 240, 0);
		m_pNotRedyFontImg[i]->SetPosition(200 + static_cast<float>(230 * i), 240, 0);
	}
}

void CSceneStandby::SetSelectorPos()
{
	m_SelectorPos.push_back(D3DXVECTOR3(455, 450, 0));
	m_SelectorPos.push_back(D3DXVECTOR3(440, 550, 0));
}

void CSceneStandby::MoveSelector()
{
	if (CInputManager::IsDown(Action::NavigateUp,0) || 0 < CInputManager::GetLeftSthikY(0))
	{
		if (m_SelectorNumber > 0)
			m_SelectorNumber--;
	}
	if (CInputManager::IsDown(Action::NavigateDown,0) || 0 > CInputManager::GetLeftSthikY(0))
	{
		if (m_SelectorNumber < m_SelectorPos.size() - 1)
			m_SelectorNumber++;
	}

	m_pSpriteSelector->SetPosition(m_SelectorPos[m_SelectorNumber]);
}
