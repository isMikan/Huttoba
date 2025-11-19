#include "Scene/SceneResult/CSceneResult.h"

#include "Camera/CameraManager/CCameraManager.h"

CSceneResult::CSceneResult()
	: m_Action				()

	, m_pSpriteResultImg	( nullptr )
	, m_pSpriteSelector		( nullptr )

	, m_pPlayerManager		()

	, m_pGroundManager		()

	, m_SelectorPos			()

	, m_SelectorNumber		( 0 )

	, isHeldUp				(false)
	, isHeldDown			(false)

	, cnt					( 0 )
{
	m_pDx11 = CDirectX11::GetInstance();

	Create();
	LoadData();
	SetSelectorPos();
}

CSceneResult::~CSceneResult()
{
}

HRESULT CSceneResult::Create()
{
	CCameraManager::SetPosition(3.f, 3.f, -10.f);
	CCameraManager::SetLook(3.f, -1.f, -2.f);
	CCameraManager::SetLight(0.f, 10.f, -10.f);

	//プレイヤーマネージャーのインスタンス作成.
	m_pPlayerManager = std::make_unique<CPlayerManager>();
	m_pPlayerManager->ResultPlayerCreate();

	//地面マネージャークラスのインスタンス作成.
	m_pGroundManager = std::make_unique<CGroundManager>();
	m_pGroundManager->ResultGroundCreate();

	m_pSpriteResultImg = std::make_unique<CUIObject>();
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
	m_pSpriteResultImg->AttachSprite(AssetManager::Sprite(Sprite2DList::Result));
	m_pSpriteSelector->AttachSprite(AssetManager::Sprite(Sprite2DList::Selector));
	
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

	//地面に接地しているか.
	for (auto& player : m_pPlayerManager->GetPlayer())
	{
		if (!player) continue;	//プレイヤーがいない場合、次へ.

		player->OnGroundCollision(*m_pGroundManager);
	}

	//プレイヤーの動作.
	m_pPlayerManager->ResultPlayerUpdate();

	//関数を入れる
	m_Action =
	{
		//ラムダ式で関数にしてm_Actionの中に入れている(SetNextScene(Standby);ではだめ).
		//画面に表示される選択肢の文字と同じ順番に処理を入れていく
		[this]() {SetNextScene(GameMain);},
		[this]() {SetNextScene(Standby);},
		[this]() {SetNextScene(Title);}
	};

	if (CInputManager::IsDown(Action::Decide,0))
	{
		//選択中の番号で処理される関数が変わる.
		m_Action[m_SelectorNumber]();
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
	m_pSpriteSelector->Draw();
	m_pSpriteResultImg->Draw();
	m_pDx11->SetDepth(true);
}

void CSceneResult::Destroy()
{
}

void CSceneResult::SetSelectorPos()
{
	m_SelectorPos.push_back(D3DXVECTOR3(790, 415, 0));
	m_SelectorPos.push_back(D3DXVECTOR3(750, 510, 0));
	m_SelectorPos.push_back(D3DXVECTOR3(770, 590, 0));
}

void CSceneResult::MoveSelector()
{
	if (CInputManager::IsDown(Action::NavigateUp,0)/* || 0 < m_InputManager.GetInput(0).GetLeftSthikY()*/)
	{
		if (m_SelectorNumber > 0)
			m_SelectorNumber--;
	}
	if (CInputManager::IsDown(Action::NavigateDown,0)/* || 0 > m_InputManager.GetInput(0).GetLeftSthikY()*/)
	{
		if (m_SelectorNumber < m_SelectorPos.size() - 1)
			m_SelectorNumber++;
	}

	m_pSpriteSelector->SetPosition(m_SelectorPos[m_SelectorNumber]);
}

void CSceneResult::SelectorControl()
{
	float stickY = CInputManager::GetLeftSthikY(0);

	// パラメータ
	const float threshold = 0.5f; // 入力と判定するスティックの倒し量
	const float initialDelay = 0.2f; // 最初のディレイ（秒）
	const float repeatInterval = 0.1f; // リピート間隔（秒）
	const float dt = 1.0f / 60.0f; // 固定FPSならこれでOK

	static float holdTimerUp = 0.0f;
	static float holdTimerDown = 0.0f;

	// 上方向
	if (stickY > threshold) {
		if (!isHeldUp) {
			if (m_SelectorNumber > 0)
				m_SelectorNumber--;
			isHeldUp = true;
			holdTimerUp = initialDelay;
		}
		else {
			holdTimerUp -= dt;
			if (holdTimerUp <= 0.0f) {
				if (m_SelectorNumber > 0)
					m_SelectorNumber--;
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
				m_SelectorNumber++;
			isHeldDown = true;
			holdTimerDown = initialDelay;
		}
		else {
			holdTimerDown -= dt;
			if (holdTimerDown <= 0.0f) {
				if (m_SelectorNumber < m_SelectorPos.size() - 1)
					m_SelectorNumber++;
				holdTimerDown = repeatInterval;
			}
		}
	}
	else
	{
		isHeldDown = false;
	}
}