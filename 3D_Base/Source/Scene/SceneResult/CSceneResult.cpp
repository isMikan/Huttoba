#include "Scene/SceneResult/CSceneResult.h"

CSceneResult::CSceneResult()
	: m_Action				()

	, m_ResultFontIndex		( 3 )

	, m_pSpriteResultFont	()
	, m_pSpriteResultUIFont	()
	, m_pSpriteSelector		( nullptr )

	, m_pPlayerManager		()

	, m_pGroundManager		()

	, m_FontPatternNo		()
	, m_FontPos				()

	, m_SelectorPos			()

	, m_SelectorNumber		( 0 )

	, isHeldUp				(false)
	, isHeldDown			(false)

	, cnt					( 0 )

	, m_FireworksEffect		()
	, m_FireworksMax		( 6 )
{
	m_pDx11 = CDirectX11::GetInstance();

	Create();
	LoadData();
	SetSelectorPos();

	m_FireworksEffect.resize(m_FireworksMax);
}

CSceneResult::~CSceneResult()
{
	for(auto& firework : m_FireworksEffect)
	{
		AssetManager::Effect()->Stop(firework);
	}
	AssetManager::Sound()->Stop(enSoundList::BGM_SceneResult);
}

HRESULT CSceneResult::Create()
{
	CCameraManager::SetPosition(3.f, 3.f, -10.f);
	CCameraManager::SetLook(3.f, 0.f, -2.f);
	CCameraManager::SetLight(0.f, 10.f, -10.f);

	//プレイヤーマネージャーのインスタンス作成.
	m_pPlayerManager = std::make_unique<CPlayerManager>();
	m_pPlayerManager->ResultPlayerCreate();

	//地面マネージャークラスのインスタンス作成.
	m_pGroundManager = std::make_unique<CGroundManager>();
	m_pGroundManager->ResultGroundCreate();

	m_pSpriteResultFont = std::make_unique<CUIObject>();

	for (int i = 0;i < m_ResultFontIndex;i++)
	{
		m_pSpriteResultUIFont.push_back(std::make_unique<CUIObject>());
	}

	m_pSpriteSelector = std::make_unique<CUIObject>();

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

HRESULT CSceneResult::LoadData()
{
	SetFontPattern();

	SetFontPos();

	m_pSpriteResultFont->AttachSprite(AssetManager::Sprite(Sprite2DList::Font_Result));

	//生き残ったプレイヤーが複数いたら
	if (CSceneData::GetPlayerLivingNum() != 1)
	{
		//DRAWの表示
		m_pSpriteResultFont->SetPatternNo(0, 1);

		//表示数位置の調整
		m_pSpriteResultFont->SetPosition(D3DXVECTOR3(50, WND_H - 330, 0));
	}
	else
	{
		//1以下ならWINNERを表示
		m_pSpriteResultFont->SetPatternNo(0, 0);

		//表示数位置の調整
		m_pSpriteResultFont->SetPosition(D3DXVECTOR3(50, WND_H - 280, 0));
	}

	for (int i=0;i< m_ResultFontIndex;i++)
	{
		m_pSpriteResultUIFont[i]->AttachSprite(AssetManager::Sprite(Sprite2DList::Font_UI_Common));

		m_pSpriteResultUIFont[i]->SetPatternNo(0, m_FontPatternNo[i]);

		m_pSpriteResultUIFont[i]->SetPosition(m_FontPos[i]);
	}
	m_pSpriteSelector->AttachSprite(AssetManager::Sprite(Sprite2DList::UI_Selector));
	
	//プレイヤーマネージャーの読み込み.
	m_pPlayerManager->LoadData();

	//地面マネージャーの読み込み.
	m_pGroundManager->LoadData();
	
	return S_OK;
}


void CSceneResult::Update()
{
	MoveSelector();

	SelectorControl();

	AssetManager::Sound()->PlayLoop(enSoundList::BGM_SceneResult);

	//地面に接地しているか.
	for (auto& player : m_pPlayerManager->GetPlayer())
	{
		if (!player) continue;	//プレイヤーがいない場合、次へ.

		player->OnGroundCollision(*m_pGroundManager);
	}

	//プレイヤーの動作.
	m_pPlayerManager->ResultPlayerUpdate();

	for(auto& player : m_pPlayerManager->GetPlayer())
	{
		for (int eNo = 0; eNo < m_FireworksMax; eNo++)
		{
			if (CSceneData::GetPlayerLiving(player->GetPlayerID())
				&& !AssetManager::Effect()->IsPlaying(m_FireworksEffect[eNo]))
			{
				int posX = rand() % 10 - 1;	//-1～8までの数値.
				m_FireworksEffect[eNo] = AssetManager::Effect()->Play("Fireworks", D3DXVECTOR3(posX, -4.f, 1.f));
				AssetManager::Effect()->SetScale(m_FireworksEffect[eNo], D3DXVECTOR3(0.3f, 0.3f, 0.3f));
			}
		}
	}

	if (CInputManager::IsDown(Action::Decide,0))
	{
		//選択中の番号で処理される関数が変わる.
		m_Action[m_SelectorNumber]();
		AssetManager::Sound()->PlaySE(enSoundList::SE_Decision);
	}
}


void CSceneResult::Draw()
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

	for (auto& Font : m_pSpriteResultUIFont)
	{
		Font->Draw();
	}

	m_pSpriteResultFont->Draw();

	m_pSpriteSelector->Draw();

	CFadeManager::Draw(0.f, 1.f, true);

	m_pDx11->SetDepth(true);

	//Effectクラス
	AssetManager::Effect()->Draw(view, proj, light, camera);
}

void CSceneResult::Destroy()
{
}

void CSceneResult::SetFontPattern()
{
	m_FontPatternNo.push_back(4);
	m_FontPatternNo.push_back(5);
	m_FontPatternNo.push_back(3);
}

void CSceneResult::SetFontPos()
{
	m_FontPos.push_back(D3DXVECTOR3(920, 500, 0));
	m_FontPos.push_back(D3DXVECTOR3(920, 560, 0));
	m_FontPos.push_back(D3DXVECTOR3(920, 630, 0));
}

void CSceneResult::SetSelectorPos()
{
	m_SelectorPos.push_back(D3DXVECTOR3(850, 500, 0));
	m_SelectorPos.push_back(D3DXVECTOR3(850, 560, 0));
	m_SelectorPos.push_back(D3DXVECTOR3(850, 630, 0));
}

void CSceneResult::MoveSelector()
{
	if (CInputManager::IsDown(Action::NavigateUp, 0)/* || 0 < m_InputManager.GetInput(0).GetLeftSthikY()*/)
	{
		if (m_SelectorNumber > 0)
		{
			m_SelectorNumber--;
			AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
		}
	}
	if (CInputManager::IsDown(Action::NavigateDown,0)/* || 0 > m_InputManager.GetInput(0).GetLeftSthikY()*/)
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

	//pos.yを拡縮に合わせて少し下にずらす
	pos.y += 32.f * (1 - sin);


	m_pSpriteSelector->SetPosition(pos);
}

void CSceneResult::SelectorControl()
{
	float stickY = CInputManager::GetLeftSthikY(0);

	//パラメータ
	const float threshold = 0.5f;		//入力と判定するスティックの倒し量
	const float initialDelay = 0.2f;	//最初のディレイ（秒）
	const float repeatInterval = 0.1f;	//リピート間隔（秒）

	float dt = CTimeManager::GetDeltaTime();

	static float holdTimerUp = 0.0f;
	static float holdTimerDown = 0.0f;

	// 上方向
	if (stickY > threshold) {
		if (!isHeldUp) {
			if (m_SelectorNumber > 0)
			{
				m_SelectorNumber--;
				AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
			}
			isHeldUp = true;
			holdTimerUp = initialDelay;
		}
		else {
			holdTimerUp -= dt;
			if (holdTimerUp <= 0.0f) {
				if (m_SelectorNumber > 0)
				{
					m_SelectorNumber--;
					AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
				}
				holdTimerUp = repeatInterval;
			}
		}
	}
	else {
		isHeldUp = false;
	}

	// 下方向
	if (stickY < -threshold) {
		if (!isHeldDown) {
			if (m_SelectorNumber < m_SelectorPos.size() - 1)
			{
				m_SelectorNumber++;
				AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
			}
			isHeldDown = true;
			holdTimerDown = initialDelay;
		}
		else {
			holdTimerDown -= dt;
			if (holdTimerDown <= 0.0f) {
				if (m_SelectorNumber < m_SelectorPos.size() - 1)
				{
					m_SelectorNumber++;
					AssetManager::Sound()->PlaySE(enSoundList::SE_MoveSelectionArrow);
				}
				holdTimerDown = repeatInterval;
			}
		}
	}
	else
	{
		isHeldDown = false;
	}
}