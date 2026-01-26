#include "CSceneTitle.h"

#include "Scene/SceneData/CSceneData.h"

CSceneTitle::CSceneTitle(HWND hWnd)
	: m_hWnd			(hWnd)

	, m_Action			()

	, m_pPlayerManager	()
	, m_pGroundManager	()

	, m_pSpriteTitleImg	()
	, m_pSpriteSelector	()

	, m_pSpriteStartFont()
	, m_pSpriteEndFont	()

	, m_SelectorPos		()

	, m_SelectorNumber	( 0 )

	, m_StartTime		()
	, m_EndTime			( 30.f )
	//, m_SthikThreshold	(0.5f)		//ここの値を変えると選択肢を動かす
									//スティックの最低値が変化する.

{	
	m_pDx11 = CDirectX11::GetInstance();

	CSceneData::PlayerAllLive();
	CSceneData::AllSlotStandby();

	Create();
	LoadData();
	SetSelectorPos();
}

CSceneTitle::~CSceneTitle()
{
	AssetManager::Sound()->Stop(enSoundList::BGM_SceneTitle);
}

HRESULT CSceneTitle::Create()
{
	//地面マネージャークラスのインスタンス作成.
	m_pGroundManager = std::make_unique<CGroundManager>();
	m_pGroundManager->MainGroundCreate();

	//プレイヤーマネージャーのインスタンス作成.
	m_pPlayerManager = std::make_unique<CPlayerManager>();

	m_pSpriteTitleImg = std::make_unique<CUIObject>();
	m_pSpriteSelector = std::make_unique<CUIObject>();

	//m_pSpriteTitleFont = std::make_unique<CUIObject>();
	m_pSpriteStartFont = std::make_unique<CUIObject>();
	m_pSpriteEndFont = std::make_unique<CUIObject>();

	//アイテムマネージャーの作成
	m_pItemManager = std::make_unique<ItemManager>(m_pGroundManager);
	m_pPlayerManager->MainPlayerCreate(m_pItemManager.get(), m_pGroundManager.get());

	return S_OK;
}

HRESULT CSceneTitle::LoadData()
{
	CCameraManager::SetPosition(0.f, 2.f, -8.f);
	CCameraManager::SetLook(0.f, 1.f, 5.f);
	CCameraManager::SetLight(0.f, 30.f, -10.f);

	//プレイヤーマネージャーの読み込み.
	m_pPlayerManager->LoadData();

	//地面マネージャーの読み込み.
	m_pGroundManager->LoadData();
	
	m_pSpriteTitleImg->AttachSprite(AssetManager::Sprite(Sprite2DList::Font_Title));
	m_pSpriteTitleImg->SetPosition(300.f, 20.f, 0.f);

	m_pSpriteSelector->AttachSprite(AssetManager::Sprite(Sprite2DList::UI_Selector));

	m_pSpriteStartFont->AttachSprite(AssetManager::Sprite(Sprite2DList::Font_UI_Common));
	m_pSpriteStartFont->SetPatternNo(0, 0);
	m_pSpriteStartFont->SetPosition(550, 440, 0);

	m_pSpriteEndFont->AttachSprite(AssetManager::Sprite(Sprite2DList::Font_UI_Common));
	m_pSpriteEndFont->SetPatternNo(0, 1);
	m_pSpriteEndFont->SetPosition(570, 540, 0);

	//m_pItemManager->LoadData();

	//関数を入れる
	m_Action =
	{
		//ラムダ式で関数にしてm_Actionの中に入れている[this]でメンバ関数が触れるようになる.
		//画面に表示される選択肢の文字と同じ順番に処理を入れていく
		[this]() {SetNextScene(Standby);},
		[this]() {DestroyWindow(m_hWnd);}
	};

	return S_OK;
}

void CSceneTitle::Update()
{
	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	//BGMのループ再生
	AssetManager::Sound()->PlayLoop(enSoundList::BGM_SceneTitle);

	//地面に接地しているか
	for (auto& player : m_pPlayerManager->GetPlayer())
	{
		if (!player) continue;	//プレイヤーがいない場合、次へ

		player->OnGroundCollision(*m_pGroundManager);
	}

	//プレイヤーの動作
	m_pPlayerManager->TitlePlayerUpdate();
	//\
	m_pPlayerManager->Update();

	//画面がどれくらいのフェードから操作できるかを指定している
	if (CFadeManager::GetAlpha() >= 0.7f)return;

	MoveSelector();

	if (CInputManager::IsDown(Action::Decide,0))
	{
		//選択中の番号で処理される関数が変わる.
		m_Action[m_SelectorNumber]();
		AssetManager::Sound()->PlaySE(enSoundList::SE_Decision);
	}
	CollisionManager::GetInstance()->Update();

	//m_pItemManager->Update();
}

void CSceneTitle::Draw()
{
	//カメラの処理.
	CCameraManager::Update();

	//経過時間を取得.
	float t = CTimeManager::GetTotalTime();

	static float baseAngle = 0.f;	//ベースの角度.

	static D3DXVECTOR3 cameraPos;
	if(t < 4.f)
	{
		cameraPos = D3DXVECTOR3(0.f, 15.f, -15.f);		
		baseAngle = atan2f(cameraPos.z, cameraPos.x);	//元の角度からずれないように.
	}
	//開始時間が終了時間を超えた場合.
	if (t - m_StartTime > m_EndTime)
	{
		m_StartTime = t;
	}
	//全体の時間の現在の割合.
	float progress = (t - m_StartTime) / m_EndTime;
	progress = std::clamp(progress, 0.f, 1.f);

	float angle = baseAngle + progress * D3DX_PI * 2.f;	//ベースの角度を加え一周する.
		
	cameraPos.x = cosf(angle) * 25.f;
	cameraPos.z = sinf(angle) * 25.f + 10.f;	//奥にするとちょうどいいので 10 足す.

	//カメラを動かす処理.
	CCameraManager::PositionUpdate(
		cameraPos, D3DXVECTOR3(0.f, 0.5f, 10.f));

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
	
	m_pItemManager->Draw(view, proj, light, camera);

	m_pDx11->SetDepth(false);

	//m_pSpriteTitleFont->Draw();
	m_pSpriteStartFont->Draw();
	m_pSpriteEndFont->Draw();

	m_pSpriteSelector->Draw();

	m_pSpriteTitleImg->Draw();

	m_pDx11->SetDepth(false);

	static float startTime;
	static bool isFade = false;
	//プレイヤーが一人の場合.
	if (CSceneData::GetPlayerLivingNum() <= 0)
	{
		if (!isFade)
		{
			startTime = t;
			isFade = true;
		}

		CFadeManager::Draw(startTime, 1.f, false);

		if (t - startTime > 2.f)
		{
			CSceneData::PlayerAllLive();
		}
	}
	else
	{
		if (isFade)
		{
			isFade = false;
		}
	}

	CFadeManager::Draw(0.f, 1.f, true);

	m_pDx11->SetDepth(true);
}

void CSceneTitle::Destroy()
{
}

void CSceneTitle::SetSelectorPos()
{
	m_SelectorPos.push_back(D3DXVECTOR3(465, 440, 0));
	m_SelectorPos.push_back(D3DXVECTOR3(490, 540, 0));

	m_pSpriteSelector->SetPosition(m_SelectorPos[m_SelectorNumber]);
}

void CSceneTitle::MoveSelector()
{
	if (CInputManager::IsDown(Action::NavigateUp, 0) || 0 < CInputManager::GetLeftSthikY(0))
	{
		if (m_SelectorNumber > 0)
		{
			m_SelectorNumber--;
			AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
		}
	}
	if (CInputManager::IsDown(Action::NavigateDown, 0) || 0 > CInputManager::GetLeftSthikY(0))
	{
		if (m_SelectorNumber < m_SelectorPos.size() - 1)
		{
			m_SelectorNumber++;
			AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
		}
	}
	m_pSpriteSelector->SetPosition(m_SelectorPos[m_SelectorNumber]);


	//0～1の間の数値を計算
	float sin = fabsf(sinf(CTimeManager::GetTotalTime() * 3));

	//拡縮設定
	m_pSpriteSelector->SetScale(1.0f, sin, 1.0f);

	//設置位置
	D3DXVECTOR3 pos = m_SelectorPos[m_SelectorNumber];

	//設置位置のy軸を拡縮に合わせて少し下にずらす
	pos.y += 32.f * (1 - sin);

	m_pSpriteSelector->SetPosition(pos);
}
