#include "CSceneStandby.h"

#include "Camera/CameraManager/CCameraManager.h"
#include "Scene/SceneData/CSceneData.h"

CSceneStandby::CSceneStandby(std::unordered_map<ItemID, bool>& SpawnItemData)
	: m_pRedyFontImg		()
	, m_pNotRedyFontImg		()

	, m_pSpriteSelector		(nullptr)

	, m_pPlayerManager		()

	, m_pGroundManager		()

	, m_pSpriteStartFont	()
	, m_pSpriteEndFont		()	

	, m_Action				()

	, m_SelectorPos			()

	, m_HorizontalSelectorNumber		( 0 )
	, m_SelectSpawnItemData	(SpawnItemData)
{
	m_pDx11 = CDirectX11::GetInstance();

	CSceneData::PlayerAllLive();
	CSceneData::AllSlotStandby();

	Create();
	LoadData();
	InitializeRedyFont();
	SetSelectorPos();
}

CSceneStandby::~CSceneStandby()
{
	AssetManager::Sound()->Stop(enSoundList::BGM_SceneStanby);
}

HRESULT CSceneStandby::Create()
{
	CCameraManager::SetPosition(2.f, 3.f, -8.f);
	CCameraManager::SetLook(2.f, 0.f, 0.f);
	CCameraManager::SetLight(0.f, 10.f, -10.f);

	//プレイヤーマネージャーのインスタンス作成.
	m_pPlayerManager = std::make_unique<CPlayerManager>();
	m_pPlayerManager->StandbyPlayerCreate();

	//地面マネージャークラスのインスタンス作成.
	m_pGroundManager = std::make_unique<CGroundManager>();
	m_pGroundManager->ResultGroundCreate();
	
	m_pSpriteStartFont = std::make_unique<CUIObject>();
	m_pSpriteEndFont = std::make_unique<CUIObject>();

	for (size_t i = 0;i < 4;i++)
	{
		m_pRedyFontImg[i] = std::make_unique<CUIObject>();
		m_pNotRedyFontImg[i] = std::make_unique<CUIObject>();
	}

	m_pSpriteSelector = std::make_unique<CUIObject>();

	return S_OK;
}

HRESULT CSceneStandby::LoadData()
{
	for (size_t i = 0;i < 4;i++)
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
	m_pSpriteStartFont->SetScale(1.1f, 1.1f, 1.f);

	m_pSpriteEndFont->AttachSprite(AssetManager::Sprite(Sprite2DList::Font_UI_Common));
	m_pSpriteEndFont->SetPatternNo(0, 3);
	m_pSpriteEndFont->SetPosition(500, 540, 0);

	//関数を入れる
	//ラムダ式で関数にしてm_Actionの中に入れている(SetNextScene(Standby);ではだめ).
	//画面に表示される選択肢の文字と同じ順番に処理を入れていく
	m_Action =
	{
		{
			[this]() { SetNextScene(GameMain);	},
			//すべてONを追加予定
			[this]() { SwitchSpawnFlag(ItemID::Bomb); },
			[this]() { SwitchSpawnFlag(ItemID::Boomerang); },
			[this]() { SwitchSpawnFlag(ItemID::Fun); },

		},
		{
			[this]() { SetNextScene(Title);		},
			[this]() { SwitchSpawnFlag(ItemID::Haetataki); },
			[this]() { SwitchSpawnFlag(ItemID::Mushroom); },
			[this]() { SwitchSpawnFlag(ItemID::SmashBat); },
			[this]() { SwitchSpawnFlag(ItemID::TrackingRobot); },

		}

	};

	//出現するアイテム設定の初期化
	m_SelectSpawnItemData =
	{
		{ItemID::Bomb,			true},
		{ItemID::Boomerang,		true},
		{ItemID::Fun,			true},
		{ItemID::Haetataki,		true},
		{ItemID::Mushroom,		true},
		{ItemID::SmashBat,		true},
		{ItemID::TrackingRobot,	true},
	};

	//初期化

	return S_OK;
}

void CSceneStandby::Update()
{
	AssetManager::Sound()->PlayLoop(enSoundList::BGM_SceneStanby);

	//画面がどれくらいのフェードから操作できるかを指定している
	if (CFadeManager::GetAlpha() <= 0.7f)
	{
		MoveSelector();

		if (CInputManager::IsDown(Action::Decide, 0))
		{
			switch (m_HorizontalSelectorNumber)
			{
			case 0:
				//コントローラー番号0が準備OKなら
				if (CSceneData::GetSlot(0))
				{
					//選択中の番号で処理される関数が変わる.
					m_Action[m_VerticalSelectorNumber][m_HorizontalSelectorNumber]();
				}
				break;
			case 1:
				//選択中の番号で処理される関数が変わる.
				m_Action[m_VerticalSelectorNumber][m_HorizontalSelectorNumber]();
				break;
			default:
				break;
			}
		}

		//コントローラーで準備状態切り替え.
		for (size_t i = 0; i < 4; ++i)
		{
			if (CInputManager::IsDown(Action::Switch, i))
			{
				CSceneData::ChangeSlot(i);
			}
		}
	}

	//プレイヤーの動作
	m_pPlayerManager->StandbyPlayerUpdate();
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

	for (size_t i = 0;i < 4;i++)
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


	//コントローラー番号0が準備OKなら
	if (CSceneData::GetSlot(0))	m_pSpriteStartFont->SetAlpha(1.f);
	else m_pSpriteStartFont->SetAlpha(0.5f);

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
	for (size_t i = 0;i < 4;i++)
	{
		m_pRedyFontImg[i]->SetPosition(170.f + (260.f * i), 100.f, 0.f);
		m_pNotRedyFontImg[i]->SetPosition(170.f + (260.f * i), 100.f, 0.f);
	}
}

void CSceneStandby::SetSelectorPos()
{
	m_SelectorPos.push_back(D3DXVECTOR3(455, 450, 0));
	m_SelectorPos.push_back(D3DXVECTOR3(440, 550, 0));

	m_pSpriteSelector->SetPosition(m_SelectorPos[m_HorizontalSelectorNumber]);
}

void CSceneStandby::MoveSelector()
{
	if (CInputManager::IsDown(Action::NavigateUp, 0) || 0 < CInputManager::GetLeftSthikY(0))
	{
		AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
		if (m_HorizontalSelectorNumber > 0)
			m_HorizontalSelectorNumber--;
	}
	if (CInputManager::IsDown(Action::NavigateDown, 0) || 0 < CInputManager::GetLeftSthikY(0))
	{
		AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
		if (m_HorizontalSelectorNumber < m_SelectorPos.size() - 1)
			m_HorizontalSelectorNumber++;
	}

	//0～1の間の数値を計算
	float sin = fabsf(sinf(CTimeManager::GetTotalTime() * 3));

	//拡縮設定
	m_pSpriteSelector->SetScale(1.0f, sin, 1.0f);

	//設置位置
	D3DXVECTOR3 pos = m_SelectorPos[m_HorizontalSelectorNumber];

	//pos.yを拡縮に合わせて少し下にずらす
	pos.y += 32.f * (1 - sin);


	m_pSpriteSelector->SetPosition(pos);
}

void CSceneStandby::SwitchSpawnFlag(ItemID SpawnItemData)
{
	m_SelectSpawnItemData[SpawnItemData] == true ? false : true;
}

void CSceneStandby::SwitchAllSpawnFlag()
{
	//一つでもtrueの場合はすべてfalseに
	//すべてfalseの場合はすべてtrueに

	for (auto itemSpawnFrag : m_SelectSpawnItemData)
	{
		//trueの場合は続行
		if (itemSpawnFrag.second) continue;

		for (auto itemSpawnFrag : m_SelectSpawnItemData)
		{
			itemSpawnFrag.second = true;
		}
		break;
	}
}
