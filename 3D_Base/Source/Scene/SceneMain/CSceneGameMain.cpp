#include "CSceneGameMain.h"

#include "Camera/CameraManager/CCameraManager.h"
#include "Assets/Effect/CEffect.h"
#include "Assets/Sound/CSoundManager.h"
#include "Item/ItemManager/ItemManager.h"
#include "Item/ItemBase.h"
#include "Collision/CollisionDraw/CollisionDraw.h"
#include "Scene/SceneData/CSceneData.h"

CSceneGameMain::CSceneGameMain( HWND hWnd)
	: m_hWnd			( hWnd )

	, m_GameState		( GameState::Ready )

	, m_StateTimer		( 0.0f )

	, m_ReadyDuration	( 1.0f )
	, m_FinishDuration	( 2.0f )

	, m_pDbgText		( nullptr )

	, m_pExplosiones	()

	, m_pShadowManager	()

	, m_pGaugeManager	()

	, m_pPlayerManager	()

	, m_pGroundManager	()

	, m_pItemManager	()
	, m_IsPause			( false )

	, m_pGroundCollisionProxy	()

	, m_pSpriteReadyGo	()
{
	m_pDx9 = CDirectX9::GetInstance();
	m_pDx11 = CDirectX11::GetInstance();

	CSceneData::PlayerAllLive();

	CInputManager::BindKey(Action::Pause, InputBinding(InputDevice::Keyboard, VK_F3), 0);

	Create();
	LoadData();
}

CSceneGameMain::~CSceneGameMain()
{
	//外部で作成しているので、ここでは破棄しない
	m_hWnd = nullptr;

	//登録している当たり判定をすべて削除
	CollisionManager::GetInstance()->Init();
}

HRESULT CSceneGameMain::Create()
{
	//あらかじめ領域確保
	m_pExplosiones.resize(Explosion_Max);

	//デバッグテキストのインスタンス作成
	m_pDbgText = std::make_unique<CDebugText>();

	//影マネージャーのインスタンス作成
	m_pShadowManager = std::make_unique<CShadowManager>();

	m_pDrawTimer = std::make_unique<DrawTimer>();

	//プレイヤーマネージャーのインスタンス作成.
	m_pPlayerManager = std::make_unique<CPlayerManager>();

	//地面マネージャークラスのインスタンス作成.
	m_pGroundManager = std::make_unique<CGroundManager>();
	m_pGroundManager->MainGroundCreate();

	//アイテムマネージャーの作成
	m_pItemManager = std::make_unique<ItemManager>(m_pGroundManager);

	m_pPlayerManager->MainPlayerCreate(m_pItemManager.get());

	//地面の当たり判定クラスの生成
	m_pGroundCollisionProxy = std::make_unique<CGroundCollisionProxy>(*m_pGroundManager);

	//各オブジェクトのインスタンス作成
	CteateExplosion();

	//ゲージマネージャーのインスタンス作成.
	m_pGaugeManager = std::make_unique<CGaugeManager>(m_pItemManager.get());
	//ゲージを作成.
	m_pGaugeManager->Create(m_pPlayerManager.get());

	m_pSpriteReadyGo = std::make_unique<CUIObject>();

	return S_OK;
}

HRESULT CSceneGameMain::LoadData()
{
	//デバッグテキストの読み込み
	if (FAILED(m_pDbgText->Init())) {
		return E_FAIL;
	}

	CCameraManager::SetPosition(0.f, 20.f, -10.f);
	CCameraManager::SetLook( 0.f, 2.f, 6.f );
	CCameraManager::SetLight( 0.f, 30.f, -10.f );

	//爆発スプライトを設定.
	for (const auto& exp : m_pExplosiones)
	{
		exp->AttachSprite(AssetManager::Sprite(Sprite3DList::Explosion));
	}

	//影マネージャーの読み込み.
	m_pShadowManager->LoadData();

	//プレイヤーマネージャーの読み込み.
	m_pPlayerManager->LoadData();

	//地面マネージャーの読み込み.
	m_pGroundManager->LoadData();

	m_pItemManager->LoadData();

	//画像データの読み込み
	m_pSpriteReadyGo->AttachSprite(AssetManager::Sprite(Sprite2DList::ReadyGo));

	return S_OK;
}

void CSceneGameMain::Init()
{
	m_pItemManager->Init();
}

void CSceneGameMain::Destroy()
{
}

void CSceneGameMain::Update()
{
	//BGMのループ再生
	AssetManager::Sound()->PlayLoop(enSoundList::BGM_SceneMain);
	if (CInputManager::IsDown(Action::Pause, 0))
	{
		if (!m_IsPause)
		{
			CTimeManager::Pause();
			m_IsPause = true;
		}
		else
		{
			CTimeManager::Resume();
			m_IsPause = false;
		}
	}

	//経過時間を計算
	m_StateTimer += CTimeManager::GetDeltaTime();

	switch (m_GameState)
	{
	case CSceneGameMain::GameState::Ready:
		if (m_StateTimer >= m_ReadyDuration)
		{
			//ゲームプレイへ
			m_GameState = GameState::Play;
			m_StateTimer = 0;
		}

		break;
	case CSceneGameMain::GameState::Play:
		//地面マネージャーの更新処理
		m_pGroundManager->Update();

		//地面に接地しているか
		for (auto& item : m_pItemManager->GetItems())
		{
			item->IsOnGround(*m_pGroundManager);
		}

		//爆発
		for (auto& exp : m_pExplosiones)
		{
			//爆発しているか
			if (exp->IsStart())
			{
				exp->Update();
			}
		}

		m_pGroundCollisionProxy->Update();

		m_pItemManager->Update();

		//プレイヤーの動作
		m_pPlayerManager->MainPlayerUpdate();

		//地面に接地しているか
		for (auto& player : m_pPlayerManager->GetPlayer())
		{
			if (!player) continue;	//プレイヤーがいない場合、次へ

			player->OnGroundCollision(*m_pGroundManager);
		}

		m_pDrawTimer->Update();
		m_pShadowManager->Update(m_pPlayerManager.get(), m_pItemManager.get());
		m_pGaugeManager->Update();

		//レーザーの管理
		ManageEffectLaser();

		CollisionManager::GetInstance()->Update();

		//次のシーンへ遷移
		if (GetAsyncKeyState(VK_F4) & 0x8000)
		{
			SetNextScene(Result);
		}

		//プレイヤーが一人以下の場合.
		if (CSceneData::GetPlayerLivingNum() <= 1)
		{
			//SetNextScene(Result);
			m_GameState = GameState::Finish;
			m_StateTimer = 0;
		}

		if (m_StateTimer >= 60.0f)
		{
			//フィニッシュへ
			m_GameState = GameState::Finish;
			m_StateTimer = 0;
		}
		break;
	case CSceneGameMain::GameState::Finish:
		//次のシーンに遷移
		if (m_StateTimer >= m_FinishDuration)
		{
			SetNextScene(Result);
		}

		break;
	default:
		break;
	}
}

void CSceneGameMain::Draw()
{
	DebugDrawManager* ddm = DebugDrawManager::GetInstance();

	//カメラの処理.
	CCameraManager::Update();
	//カメラを動かす処理.
	CCameraManager::PositionByGround(m_pGroundManager.get());

//=== 情報を取得 ===.
	CAMERA camera = CCameraManager::GetCamera();		//カメラ.
	LIGHT light = CCameraManager::GetLight();			//ライト.
	D3DXMATRIX view = CCameraManager::GetView();		//ビュー.
	D3DXMATRIX proj = CCameraManager::GetProjection();	//プロジェクション.
//==================.

#ifdef _DEBUG

	// ViewProj 行列を作成
	D3DXMATRIX mViewProj;
	D3DXMatrixMultiply(&mViewProj, &view, &proj);

	DebugDrawManager::GetInstance()->Begin(CDirectX11::GetInstance()->GetContext(), mViewProj);

	// 例 1: X/Y/Z 座標軸を描画
	float origin[] = { 0.0f, 0.0f, 0.0f };
	float x_end[] = { 5.0f, 0.0f, 0.0f };
	float y_end[] = { 0.0f, 5.0f, 0.0f };
	float z_end[] = { 0.0f, 0.0f, 5.0f };

	float red[] = { 1.0f, 0.0f, 0.0f, 1.0f }; 
	float green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	float blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };

	//ddm->DrawLine(origin, x_end, red);   // X軸 (赤)
	//ddm->DrawLine(origin, y_end, green); // Y軸 (緑)
	//ddm->DrawLine(origin, z_end, blue);  // Z軸 (青)

	// 描画実行とステート復元
	DebugDrawManager::GetInstance()->End();

#endif

	//地面マネージャーの描画.
	m_pGroundManager->Draw(view, proj, light, camera);

	//影マネージャーの描画.
	m_pShadowManager->Draw(m_pDx11, view, proj);

	m_pDrawTimer->Draw();

	//プレイヤーの描画.
	m_pPlayerManager->Draw(view, proj, light, camera);

	m_pItemManager->Draw(view, proj, light, camera);

	////ボーン座標に合わせて球体を表示
	//m_pStaticMeshMap[StaticMeshList::BSphere]->SetPosition(m_ZakoBonePos);
	//m_pStaticMeshMap[StaticMeshList::BSphere]->Render(m_mView, m_mProj, m_Light, m_Camera.vPosition);

	//深度テスト無効にすることで、処理順番で描画させることができる
	m_pDx11->SetDepth(false);

	m_pGaugeManager->Draw(view, proj);

	CFadeManager::Draw(0.f, GameMain_StartTime, true);

	//やりたいことが終わったので、深度テストを有効にしておく
	m_pDx11->SetDepth(true);

	for (auto& exp : m_pExplosiones)
	{
		exp->Draw(view, proj);
	}

	//Effectクラス
	AssetManager::Effect()->Draw(view, proj, light, camera);


//当たり判定の描画切り替え
#ifdef _DEBUG

	static bool IsDrawCollision = false;
	if (GetAsyncKeyState(VK_F5) & 0x0001) 
	{ 
		IsDrawCollision == true ? IsDrawCollision = false : IsDrawCollision = true;
	}

	if (IsDrawCollision)
	{
		// シングルトンとして呼び出しに戻す
		CDirectX11::GetInstance()->SetRasterizerWireframe();
		CollisionDraw::GetInstance()->Draw(view, proj, light, camera);
		CDirectX11::GetInstance()->SetRasterizerSolid();
	}

#endif // DEBUG

	m_pDx11->SetDepth(false);
	switch (m_GameState)
	{
	case CSceneGameMain::GameState::Ready:
		m_pSpriteReadyGo->Draw();
		break;
	case CSceneGameMain::GameState::Finish:
		m_pSpriteReadyGo->Draw();
		break;
	default:
		break;
	}
	m_pDx11->SetDepth(true);
}

HRESULT CSceneGameMain::CteateExplosion()
{
	//爆発クラスのインスタンス作成.
	for (auto& exp : m_pExplosiones)
	{
		exp = std::make_unique<CExplosion>();
		if (!exp) return E_POINTER;
	}
	return E_NOTIMPL;
}

void CSceneGameMain::ManageEffectLaser()
{
	//ほんとはメンバ変数で作ってあげる
	//エフェクトのインスタンスごとに必要になるハンドル
	//※3つ制御するなら3つ必要
	//static ::EsHandle hEffect = -1;

	//if (GetAsyncKeyState('Y') & 0x0001)
	//{
	//	for (auto& player : m_pPlayers)
	//	{
	//		hEffect = AssetManager::Effect()->Play("Laser", player->GetPosition());
	//	}

	//	//拡縮
	//	AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	//	AssetManager::Effect()->SetRotation(hEffect, D3DXVECTOR3(D3DXToRadian(-90.f), 0.f, 0.f));
	//	AssetManager::Effect()->SetLocation(hEffect, D3DXVECTOR3(0.f, 1.f, 1.f));
	//}

	////Effect制御
	//if (GetAsyncKeyState('T') & 0x0001)
	//{
	//	AssetManager::Effect()->Stop(hEffect);
	//}

}