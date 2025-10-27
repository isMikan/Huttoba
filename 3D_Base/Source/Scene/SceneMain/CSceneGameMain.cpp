#include "CSceneGameMain.h"
#include "Assets/Effect/CEffect.h"
#include "Assets/Sound/CSoundManager.h"
#include "Item/ItemManager/ItemManager.h"
#include "Collision/CollisionStrategy/CollisionStrategyFactory/CollisionStrategyFactory.h"
#include "Collision/CollisionStrategy/CollisionPattern/CollisionSphereSphere/CollisionStrategySphereSphere.h"

CSceneGameMain::CSceneGameMain( HWND hWnd)
	: m_hWnd			( hWnd )

	, m_pDbgText		( nullptr )

	, m_pCamera			()

	, m_pUIMap			()

	, m_pExplosiones	()

	, m_pShadowManager	()

	, m_pGaugeManager	()

	, m_pPlayerManager	()

	, m_pGroundManager	()

	, m_pItemManager	( nullptr )

	, m_pDrawCollision	()
{
	m_pDx9 = CDirectX9::GetInstance();
	m_pDx11 = CDirectX11::GetInstance();

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

	//カメラのインスタンス作成.
	m_pCamera = std::make_unique<CCamera>();

	//影マネージャーのインスタンス作成
	m_pShadowManager = std::make_unique<CShadowManager>();

	//ゲージマネージャーのインスタンス作成.
	m_pGaugeManager = std::make_unique<CGaugeManager>();

	//地面マネージャークラスのインスタンス作成.
	m_pGroundManager = std::make_unique<CGroundManager>();

	//アイテムマネージャーの作成
	m_pItemManager = std::make_unique<ItemManager>();

	m_pDrawCollision = std::make_unique<DrawCollision>();

	//各オブジェクトのインスタンス作成
	CreateUI();
	CteateExplosion();
	CreateCharactor();

	//ゲージを作成.
	m_pGaugeManager->Create(m_pPlayerManager.get());

	return S_OK;
}

HRESULT CSceneGameMain::LoadData()
{
	//デバッグテキストの読み込み
	if (FAILED(m_pDbgText->Init())) {
		return E_FAIL;
	}

	m_pCamera->SetPosition( 0.f, 10.f, -7.f );
	m_pCamera->SetLook( 0.f, -1.f, 10.f );
	m_pCamera->SetLight( 0.f, 30.f, -10.f );

	//爆発スプライトを設定.
	for (const auto& exp : m_pExplosiones)
	{
		exp->AttachSprite(AssetManager::Sprite(Sprite3DList::Explosion));
	}


	//Pモンスプライトを設定
	for (auto& UI : m_pUIMap)
	{
		UI.second->AttachSprite(AssetManager::Sprite(Sprite2DList::PMon));
	}

	//影マネージャーの読み込み.
	m_pShadowManager->LoadData();

	//プレイヤーマネージャーの読み込み.
	m_pPlayerManager->LoadData();

	//地面マネージャーの読み込み.
	m_pGroundManager->LoadData();

	m_pItemManager->LoadData();
	m_pDrawCollision->LoadData();


	//当たり判定の作成
	CollisionStrategyFactory::GetInstance()->RegisterStrategy(
		CollisionBase::ColliderType::Sphere,
		CollisionBase::ColliderType::Sphere,
		std::make_unique<CollisionStrategySphereSphere>()
	);

	return S_OK;
}

void CSceneGameMain::Init()
{
	m_pDrawCollision->Init();
	m_pItemManager->Init();
}

void CSceneGameMain::Destroy()
{
}

void CSceneGameMain::Update()
{
	//BGMのループ再生
	AssetManager::Sound()->PlayLoop(enSoundList::BGM_SceneMain);

	CTimeManager::Update();

	//地面メネージャーの更新処理.
	m_pGroundManager->Update();

	//プレイヤーの動作.
	m_pPlayerManager->Update();

	m_pItemManager->Update(m_pPlayerManager);

	CollisionManager::GetInstance()->Update();

	//爆発
	for (auto& exp : m_pExplosiones)
	{
		//爆発しているか
		if (exp->IsStart())
		{
			exp->Update();
		}
	}

	m_pDrawCollision->Update();

	//--------------------
	//	スキンメッシュ
	//--------------------
	for (auto& UI : m_pUIMap)
	{
		UI.second->Update();
	}

	m_pShadowManager->Update(m_pPlayerManager.get(), m_pItemManager.get());
	m_pGaugeManager->Update(m_pPlayerManager.get());

	//レーザーの管理
	ManageEffectLaser();

	//次のシーンへ遷移
	if (GetAsyncKeyState(VK_F4) & 0x0001)
	{
		SetNextScene(Result);
	}

}

void CSceneGameMain::Draw()
{
	DebugDrawManager* ddm = DebugDrawManager::GetInstance();

	//カメラの処理.
	m_pCamera->Update();
	//情報を取得.
	CAMERA camera = m_pCamera->GetCamera();
	LIGHT light = m_pCamera->GetLight();
	D3DXMATRIX mView = m_pCamera->GetView();
	D3DXMATRIX mProj = m_pCamera->GetProj();


#ifdef _DEBUG

	// ViewProj 行列を作成
	D3DXMATRIX mViewProj;
	D3DXMatrixMultiply(&mViewProj, &mView, &mProj);

	DebugDrawManager::GetInstance()->Begin(CDirectX11::GetInstance()->GetContext(), mViewProj);

	// 例 1: X/Y/Z 座標軸を描画
	float origin[] = { 0.0f, 0.0f, 0.0f };
	float x_end[] = { 5.0f, 0.0f, 0.0f };
	float y_end[] = { 0.0f, 5.0f, 0.0f };
	float z_end[] = { 0.0f, 0.0f, 5.0f };

	float red[] = { 1.0f, 0.0f, 0.0f, 1.0f }; 
	float green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	float blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };

	ddm->DrawLine(origin, x_end, red);   // X軸 (赤)
	ddm->DrawLine(origin, y_end, green); // Y軸 (緑)
	ddm->DrawLine(origin, z_end, blue);  // Z軸 (青)

	//std::cout << "box_max" << std::endl;

	// 描画実行とステート復元
	DebugDrawManager::GetInstance()->End();

#endif

	//地面マネージャーの描画.
	m_pGroundManager->Draw(mView, mProj, light, camera);

	//プレイヤーの描画.
	m_pPlayerManager->Draw(mView, mProj, light, camera);

	m_pItemManager->Draw(mView, mProj, light, camera);
	m_pDrawCollision->Draw(mView, mProj, light, camera);

	////ボーン座標に合わせて球体を表示
	//m_pStaticMeshMap[StaticMeshList::BSphere]->SetPosition(m_ZakoBonePos);
	//m_pStaticMeshMap[StaticMeshList::BSphere]->Render(m_mView, m_mProj, m_Light, m_Camera.vPosition);

	//深度テスト無効にすることで、処理順番で描画させることができる
	m_pDx11->SetDepth(false);

	for (auto& UI : m_pUIMap)
	{
		//UI.second->Draw();
	}

	m_pGaugeManager->Draw(mView, mProj);

	//やりたいことが終わったので、深度テストを有効にしておく
	m_pDx11->SetDepth(true);

	m_pDx11->SetAlphaBlend(true);
	m_pShadowManager->Draw(mView, mProj);
	m_pDx11->SetAlphaBlend(false);

	for (auto& exp : m_pExplosiones)
	{
		exp->Draw(mView, mProj);
	}

	////デバッグテキストの描画
	//m_pDbgText->SetColor(0.9f, 0.6f, 0.f);	//色の設定
	//m_pDbgText->Render(_T("ABCD"), 10, 100);

	////デバッグテキスト(数値入り)の描画
	//m_pDbgText->SetColor(1.f, 0.f, 0.f);
	//TCHAR dbgText[64];
	//_stprintf_s(dbgText, _T("Float:%f, %f"), 1.f, 2.2f);
	//m_pDbgText->Render(dbgText, 10, 110);

	//Effectクラス
	AssetManager::Effect()->Draw(mView, mProj, light, camera);

}

HRESULT CSceneGameMain::CreateUI()
{
	UIList UI[] =
	{
		UIList::Pmon,
	};

	for (auto& id : UI)
	{
		m_pUIMap[id] = std::make_unique<CUIObject>();
		if (!m_pUIMap[id]) return E_POINTER;
	}

	return S_OK;
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

HRESULT CSceneGameMain::CreateCharactor()
{
	//プレイヤーマネージャーのインスタンス作成.
	m_pPlayerManager = std::make_unique<CPlayerManager>();

	return S_OK;
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