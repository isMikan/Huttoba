#include "CSceneStandby.h"

#include "Camera/CameraManager/CCameraManager.h"
#include "Scene/SceneData/CSceneData.h"

CSceneStandby::CSceneStandby()
	: m_pRedyFontImg()
	, m_pNotRedyFontImg()

	, m_pSpriteSelector(nullptr)

	, m_pPlayerManager()

	, m_pGroundManager		()

	, m_pSpriteStartFont	()
	, m_pSpriteEndFont		()	

	, m_Action				()

	, m_SelectorPos()

	, m_SelectorNumber(0)
{
	m_pDx11 = CDirectX11::GetInstance();

	CSceneData::PlayerAllLive();
	CSceneData::AllSlotStandby();

	Create();
	LoadData();
	//InitializePlayers();
	InitializeRedyFont();
	SetSelectorPos();
}

CSceneStandby::~CSceneStandby()
{
	AssetManager::Sound()->Stop(enSoundList::BGM_SceneStanby);

}

HRESULT CSceneStandby::Create()
{
	CCameraManager::SetPosition(2.2f, 2.f, -6.5f);
	CCameraManager::SetLook(2.2f, 0.f, 0.f);
	CCameraManager::SetLight(0.f, 10.f, -10.f);

	//プレイヤーマネージャーのインスタンス作成.
	m_pPlayerManager = std::make_unique<CPlayerManager>();
	m_pPlayerManager->StandbyPlayerCreate();

	//地面マネージャークラスのインスタンス作成.
	m_pGroundManager = std::make_unique<CGroundManager>();
	m_pGroundManager->ResultGroundCreate();
	
	m_pSpriteStartFont = std::make_unique<CUIObject>();
	m_pSpriteEndFont = std::make_unique<CUIObject>();

	for (int i = 0;i < 4;i++)
	{
		m_pRedyFontImg[i] = std::make_unique<CUIObject>();
		m_pNotRedyFontImg[i] = std::make_unique<CUIObject>();
	}

	m_pSpriteSelector = std::make_unique<CUIObject>();

	return S_OK;
}

HRESULT CSceneStandby::LoadData()
{
	for (int i = 0;i < 4;i++)
	{
		m_pRedyFontImg[i]->AttachSprite(AssetManager::Sprite(Sprite2DList::Font_UI_Ready));
	}

	m_pSpriteSelector->AttachSprite(AssetManager::Sprite(Sprite2DList::UI_Selector));

	//プレイヤーマネージャーの読み込み.
	m_pPlayerManager->LoadData();

	//地面マネージャーの読み込み.
	m_pGroundManager->LoadData();

	m_pSpriteStartFont->AttachSprite(AssetManager::Sprite(Sprite2DList::Font_UI_Common));
	m_pSpriteStartFont->SetPatternNo(0, 2);
	m_pSpriteStartFont->SetPosition(530, 440, 0);

	m_pSpriteEndFont->AttachSprite(AssetManager::Sprite(Sprite2DList::Font_UI_Common));
	m_pSpriteEndFont->SetPatternNo(0, 3);
	m_pSpriteEndFont->SetPosition(500, 540, 0);

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

	AssetManager::Sound()->PlayLoop(enSoundList::BGM_SceneStanby);

	if (CInputManager::IsDown(Action::Decide, 0) && CSceneData::GetSlot(0))
	{
		//SetNextScene(GameMain);
		//選択中の番号で処理される関数が変わる.
		m_Action[m_SelectorNumber]();
	}

	//コントローラーで準備状態切り替え.
	for (int i = 0; i < 4; ++i)
	{
		if (CInputManager::IsDown(Action::Switch, i))
		{
			CSceneData::ChangeSlot(i);
		}
	}

	//プレイヤーの動作
	m_pPlayerManager->Update();
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

		
	//地面マネージャーの描画.
	m_pGroundManager->Draw(view, proj, light, camera);

	//プレイヤーの描画.
	m_pPlayerManager->Draw(view, proj, light, camera);

	m_pDx11->SetDepth(false);

	for (int i = 0;i < 4;i++)
	{
		if (CSceneData::GetSlot(i))
		{
			m_pRedyFontImg[i]->SetPatternNo(0, i);
		}
		else
		{
			m_pRedyFontImg[i]->SetPatternNo(0, 4 + i);
		}
		m_pRedyFontImg[i]->Draw();
	}

	m_pSpriteStartFont->Draw();
	m_pSpriteEndFont->Draw();

	m_pSpriteSelector->Draw();

	CFadeManager::Draw(0.f, 1.f, true);

	m_pDx11->SetDepth(true);
}

void CSceneStandby::Destroy()
{
}

void CSceneStandby::InitializeRedyFont()
{
	for (int i = 0;i < 4;i++)
	{
		m_pRedyFontImg[i]->SetPosition(200 + static_cast<float>(240 * i), 100, 0);
		m_pNotRedyFontImg[i]->SetPosition(200 + static_cast<float>(240 * i), 100, 0);
	}
}

void CSceneStandby::SetSelectorPos()
{
	m_SelectorPos.push_back(D3DXVECTOR3(455, 450, 0));
	m_SelectorPos.push_back(D3DXVECTOR3(440, 550, 0));
}

void CSceneStandby::MoveSelector()
{
	if (CInputManager::IsDown(Action::NavigateUp, 0) || 0 < CInputManager::GetLeftSthikY(0))
	{
		if (m_SelectorNumber > 0)
			m_SelectorNumber--;
	}
	if (CInputManager::IsDown(Action::NavigateDown, 0) || 0 > CInputManager::GetLeftSthikY(0))
	{
		if (m_SelectorNumber < m_SelectorPos.size() - 1)
			m_SelectorNumber++;
	}

	m_pSpriteSelector->SetPosition(m_SelectorPos[m_SelectorNumber]);
}
