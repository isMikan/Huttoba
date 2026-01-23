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
	, m_pReadyHowTo			()

	, m_Action				()

	, m_SelectorPos			()

	, m_HorizontalSelectorNumber		( 0 )
	, m_VerticalSelectorNumber			()
	, m_SelectSpawnItemData	(SpawnItemData)
	, m_SelectSpawnItemImg	()
	, m_ItemList			(ItemID::Bomb,ItemID::Boomerang,ItemID::Fun,ItemID::Haetataki,ItemID::Mushroom,ItemID::SmashBat,ItemID::TrackingRobot)
	, m_pItemSelector		()
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
	
	m_pSpriteStartFont	= std::make_unique<CUIObject>();
	m_pSpriteEndFont	= std::make_unique<CUIObject>();
	m_pReadyHowTo		= std::make_unique<CUIObject>();

	for (size_t i = 0;i < 4;i++)
	{
		m_pRedyFontImg[i] = std::make_unique<CUIObject>();
		m_pNotRedyFontImg[i] = std::make_unique<CUIObject>();
	}

	m_pSpriteSelector = std::make_unique<CUIObject>();


	for (auto item : m_ItemList)
	{
		m_SelectSpawnItemImg[item] = std::make_unique<CUIObject>();
	}

	m_pItemSelector = std::make_unique<CUIObject>();

	return S_OK;
}

HRESULT CSceneStandby::LoadData()
{
	for (size_t i = 0;i < 4;i++)
	{
		m_pRedyFontImg[i]->AttachSprite(AssetManager::Sprite(Sprite2DList::Font_UI_Ready));
	}

	m_pSpriteSelector->AttachSprite(AssetManager::Sprite(Sprite2DList::UI_Selector));
	m_pItemSelector->AttachSprite(AssetManager::Sprite(Sprite2DList::ItemSelecter));


	//プレイヤーマネージャーの読み込み.
	m_pPlayerManager->LoadData();

	//地面マネージャーの読み込み.
	m_pGroundManager->LoadData();

	m_pSpriteStartFont->AttachSprite(AssetManager::Sprite(Sprite2DList::Font_UI_Common));
	m_pSpriteStartFont->SetPatternNo(0, 2);
	m_pSpriteStartFont->SetPosition(530, 440, 0);
	m_pSpriteStartFont->SetScale(1.1f, 1.1f, 1.f);

	m_pReadyHowTo->AttachSprite(AssetManager::Sprite(Sprite2DList::ReadyHowTo));
	m_pReadyHowTo->SetPosition(0, 625, 0);

	m_pSpriteEndFont->AttachSprite(AssetManager::Sprite(Sprite2DList::Font_UI_Common));
	m_pSpriteEndFont->SetPatternNo(0, 3);
	m_pSpriteEndFont->SetPosition(500, 540, 0);

	m_SelectSpawnItemImg[ItemID::Bomb]			->AttachSprite(AssetManager::Sprite(Sprite2DList::ItemSelect));
	m_SelectSpawnItemImg[ItemID::Boomerang]		->AttachSprite(AssetManager::Sprite(Sprite2DList::ItemSelect));
	m_SelectSpawnItemImg[ItemID::Fun]			->AttachSprite(AssetManager::Sprite(Sprite2DList::ItemSelect));
	m_SelectSpawnItemImg[ItemID::Haetataki]		->AttachSprite(AssetManager::Sprite(Sprite2DList::ItemSelect));
	m_SelectSpawnItemImg[ItemID::Mushroom]		->AttachSprite(AssetManager::Sprite(Sprite2DList::ItemSelect));
	m_SelectSpawnItemImg[ItemID::SmashBat]		->AttachSprite(AssetManager::Sprite(Sprite2DList::ItemSelect));
	m_SelectSpawnItemImg[ItemID::TrackingRobot]	->AttachSprite(AssetManager::Sprite(Sprite2DList::ItemSelect));

	m_SelectSpawnItemImg[ItemID::Boomerang]		->SetPosition(980, 515, 0);
	m_SelectSpawnItemImg[ItemID::Fun]			->SetPosition(1060, 515, 0);
	m_SelectSpawnItemImg[ItemID::Haetataki]		->SetPosition(1140, 515, 0);
	m_SelectSpawnItemImg[ItemID::Bomb]			->SetPosition(900, 595, 0);
	m_SelectSpawnItemImg[ItemID::SmashBat]		->SetPosition(980, 595, 0);
	m_SelectSpawnItemImg[ItemID::TrackingRobot]	->SetPosition(1060, 595, 0);
	m_SelectSpawnItemImg[ItemID::Mushroom]		->SetPosition(1140, 595, 0);



	//関数を入れる
	//ラムダ式で関数にしてm_Actionの中に入れている(SetNextScene(Standby);ではだめ).
	//画面に表示される選択肢の文字と同じ順番に処理を入れていく
	m_Action =
	{
		{
			[this]() { SetNextScene(GameMain);	},
			[this]() { SwitchSpawnFlag(ItemID::Bomb); },
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
			if (m_VerticalSelectorNumber == 0 && m_HorizontalSelectorNumber == 0)
			{
				//コントローラー番号0が準備OKなら
				if (CSceneData::GetSlot(0))
				{
					//選択中の番号で処理される関数が変わる.
					m_Action[m_VerticalSelectorNumber][m_HorizontalSelectorNumber]();
				}
			}
			else
			{
				//選択中の番号で処理される関数が変わる.
				m_Action[m_VerticalSelectorNumber][m_HorizontalSelectorNumber]();
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

	for (auto& item : m_ItemList)
	{
		m_SelectSpawnItemImg[item]->Draw();

	}


	//コントローラー番号0が準備OKなら
	if (CSceneData::GetSlot(0))	m_pSpriteStartFont->SetAlpha(1.f);
	else m_pSpriteStartFont->SetAlpha(0.5f);

	m_pSpriteStartFont->Draw();
	m_pReadyHowTo->Draw();
	m_pSpriteEndFont->Draw();

	//アイテム出現欄では別のカーソルを使用するため、どちらか一つを描画する
	bool SelectSelecter = m_HorizontalSelectorNumber == 0;
	if (SelectSelecter)
	{
		m_pSpriteSelector->Draw();
	}
	else
	{
		m_pItemSelector->Draw();
	}


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

	std::vector<D3DXVECTOR3> vertical_1 =
	{
		D3DXVECTOR3(455, 440, 0),
		D3DXVECTOR3(900, 515, 0),
		D3DXVECTOR3(980, 515, 0),
		D3DXVECTOR3(1060, 515, 0),
		D3DXVECTOR3(1140, 515, 0),
	};
	std::vector<D3DXVECTOR3> vertical_2 =
	{
		D3DXVECTOR3(430, 540, 0),
		D3DXVECTOR3(900, 595, 0),
		D3DXVECTOR3(980, 595, 0),
		D3DXVECTOR3(1060, 595, 0),
		D3DXVECTOR3(1140, 595, 0),
	};


	m_SelectorPos.push_back(vertical_1);
	m_SelectorPos.push_back(vertical_2);

	m_pSpriteSelector->SetPosition(m_SelectorPos[m_VerticalSelectorNumber][m_HorizontalSelectorNumber]);
	m_pItemSelector->SetPosition(m_SelectorPos[m_VerticalSelectorNumber][m_HorizontalSelectorNumber]);
}

void CSceneStandby::MoveSelector()
{
	bool IsPushUp	= CInputManager::IsDown(Action::NavigateUp, 0)   || 0 < CInputManager::GetLeftSthikY(0); //下ボタン / 下スティックが動かされたされた？
	bool IsPushDown = CInputManager::IsDown(Action::NavigateDown, 0) || 0 > CInputManager::GetLeftSthikY(0); //上ボタン / 上スティックが動かされたされた？
	bool IsPushRight = CInputManager::IsDown(Action::NavigateRight, 0)|| 0 > CInputManager::GetLeftSthikY(0); //右ボタン / 右スティック動かされたされた？
	bool IsPushLeft = CInputManager::IsDown(Action::NavigateLeft, 0) || 0 > CInputManager::GetLeftSthikY(0); //左ボタン / 左スティックが動かされたされた？

	if (IsPushUp)
	{
		if (m_VerticalSelectorNumber > 0)
		{
			m_VerticalSelectorNumber--;
			AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
		}
	}
	if (IsPushDown)
	{
		if (m_VerticalSelectorNumber < m_SelectorPos.size() - 1)
		{
			m_VerticalSelectorNumber++;
			AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
		}
	}
	if (IsPushLeft)
	{
		if (m_HorizontalSelectorNumber > 0)
		{
			m_HorizontalSelectorNumber--;
			AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
		}
	}
	if (IsPushRight)
	{
		if (m_HorizontalSelectorNumber < m_SelectorPos[m_VerticalSelectorNumber].size() - 1)
		{
			m_HorizontalSelectorNumber++;
			AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
		}
	}

	//0～1の間の数値を計算
	float sin = fabsf(sinf(CTimeManager::GetTotalTime() * 3));

	//拡縮設定
	m_pSpriteSelector->SetScale(1.0f, sin, 1.0f);

	//設置位置
	D3DXVECTOR3 pos = m_SelectorPos[m_VerticalSelectorNumber][m_HorizontalSelectorNumber];

	m_pItemSelector->SetPosition(pos);

	//pos.yを拡縮に合わせて少し下にずらす
	pos.y += 32.f * (1 - sin);


	m_pSpriteSelector->SetPosition(pos);

}

void CSceneStandby::SwitchSpawnFlag(ItemID SpawnItemData)
{
	if (m_SelectSpawnItemData[SpawnItemData])
	{
		m_SelectSpawnItemData[SpawnItemData] = false;
	}
	else
	{
		m_SelectSpawnItemData[SpawnItemData] = true;
	}

	AssetManager::Sound()->PlaySE(enSoundList::SE_Decision);

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
