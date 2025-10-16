#include "CSceneGameMain.h"
#include "Assets/Effect/CEffect.h"
#include "Assets/Sound/CSoundManager.h"
#include "Item/ItemManager/ItemManager.h"

CSceneGameMain::CSceneGameMain( HWND hWnd, CInputManager& inputManager)
	: m_hWnd			( hWnd )

	, m_pDbgText		( nullptr )

	, m_pCamera			()

	, m_pUIMap			()

	, m_pPlayers		()

	, m_pExplosiones	()

	, m_pGroundManager	()

	, m_pItemManager	( nullptr )

	, m_pDrawCollision	()

	, m_pInputManager	( inputManager )
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
}

HRESULT CSceneGameMain::Create()
{
	//あらかじめ領域確保
	m_pExplosiones.resize(Explosion_Max);

	//デバッグテキストのインスタンス作成
	m_pDbgText = std::make_unique<CDebugText>();

	//カメラのインスタンス作成.
	m_pCamera = std::make_unique<CCamera>();

	//アイテムマネージャーの作成
	m_pItemManager = std::make_unique<ItemManager>();

	m_pDrawCollision = std::make_unique<DrawCollision>();

	//各オブジェクトのインスタンス作成
	CreateUI();
	CteateExplosion();
	CreateCharactor();

	//地面マネージャークラスのインスタンス作成.
	m_pGroundManager = std::make_unique<CGroundManager>();
	m_pGroundManager->Create();

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

	//プレイヤー.
	for (int pNo = 0;pNo < Player_Max;pNo++)
	{
		//胴体のスタティックメッシュを設定.
		m_pPlayers[pNo]->AttachMesh(AssetManager::Mesh(StaticMeshList::PBody));
		//頭のスタティックメッシュを設定.
		m_pPlayers[pNo]->GetPlayerHead().
			AttachMesh(AssetManager::Mesh(StaticMeshList::PHead));
		//右手のスタティックメッシュを設定.
		m_pPlayers[pNo]->GetPlayerRightHand().
			AttachMesh(AssetManager::Mesh(StaticMeshList::PHand));
		//左手のスタティックメッシュを設定.
		m_pPlayers[pNo]->GetPlayerLeftHand().
			AttachMesh(AssetManager::Mesh(StaticMeshList::PHand));
		//バウンディングスフィアの作成
		m_pPlayers[pNo]->CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::BSphere));

		if (!m_pInputManager.GetSlot(pNo).ready)
		{
			//キャラクターの初期座標を設定
			switch (pNo)
			{
			case 0:
				m_pPlayers[0]->SetPosition(0.f, 0.f, 0.f);
			case 1:
				m_pPlayers[1]->SetPosition(0.f, 0.f, 0.f);
			case 2:
				m_pPlayers[2]->SetPosition(0.f, 0.f, 0.f);
			case 3:
				m_pPlayers[3]->SetPosition(0.f, 0.f, 0.f);
			default:
				break;
			}
		}
	}

	//地面マネージャーの読み込み.
	m_pGroundManager->LoadData();

	m_pItemManager->LoadData();
	m_pDrawCollision->LoadData();

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

	//後でシーンマネージャーにアップデートに接地する
	CInputManager::Instance().Update();

	//地面メネージャーの更新処理.
	m_pGroundManager->Update();

	//プレイヤーの動作.
	for (auto& player : m_pPlayers)
	{
		player->Update();
		player->GetPlayerHead().Update();		//頭.
		player->GetPlayerRightHand().Update();	//右手.
		player->GetPlayerLeftHand().Update();	//左手.
	}

	//m_pItemManager->Update(m_pPlayers);

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

	//レーザーの管理
	ManageEffectLaser();

	//次のシーンへ遷移
	if (GetAsyncKeyState('L') & 0x0001)
	{
		SetNextScene(Result);
	}
}

void CSceneGameMain::Draw()
{
	//カメラの処理.
	m_pCamera->Update();
	//情報を取得.
	CAMERA camera = m_pCamera->GetCamera();
	LIGHT light = m_pCamera->GetLight();
	D3DXMATRIX mView = m_pCamera->GetView();
	D3DXMATRIX mProj = m_pCamera->GetProj();

	//地面マネージャーの描画.
	m_pGroundManager->Draw(mView, mProj, light, camera);

	//プレイヤーの描画.
	for (auto& player : m_pPlayers)
	{
		player->Draw(mView, mProj, light, camera);
		player->GetPlayerHead().Draw(mView, mProj, light, camera);		//頭.
		player->GetPlayerRightHand().Draw(mView, mProj, light, camera);	//右手.
		player->GetPlayerLeftHand().Draw(mView, mProj, light, camera);	//左手.
		//当たり判定の中心座標を更新する
		player->UpdateBSpherePos();
	}

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

	//やりたいことが終わったので、深度テストを有効にしておく
	m_pDx11->SetDepth(true);


	for (int pNo = 0;pNo < Player_Max;pNo++)
	{
		for (int aNo = 0;aNo < Player_Max;aNo++)
		{
			if (pNo == aNo) continue;

			if (m_pPlayers[aNo]->IsAttacking()
				&& m_pPlayers[aNo]->GetBSphere()->
				IsHit(*m_pPlayers[pNo]->GetBSphere()))
			{
				D3DXVECTOR3 hitPos = m_pPlayers[aNo]->GetPosition();

				m_pPlayers[pNo]->SetHitInfo(
					hitPos, hitPos, 0.05f, true, CPlayerBase::PlayerEvent::Push);

				m_pPlayers[aNo]->SetHitInfo(
					hitPos, hitPos, 0.f, true, CPlayerBase::PlayerEvent::Push);
			}
		}
	}

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
	//キャラクター関連のインスタンス作成

	//プレイヤーのインスタンス生成.
	m_pPlayers.clear();
	m_pPlayers.resize(Player_Max);
	for (int pNo = 0;pNo < Player_Max;pNo++)
	{
		m_pPlayers[pNo] = std::make_unique<CPlayer>(pNo);
		if (!m_pPlayers[pNo]) return E_POINTER;
	}

	return S_OK;
}

void CSceneGameMain::ManageEffectLaser()
{
	//ほんとはメンバ変数で作ってあげる
	//エフェクトのインスタンスごとに必要になるハンドル
	//※3つ制御するなら3つ必要
	static ::EsHandle hEffect = -1;

	if (GetAsyncKeyState('Y') & 0x0001)
	{
		for (auto& player : m_pPlayers)
		{
			hEffect = AssetManager::Effect()->Play("Laser", player->GetPosition());
		}

		//拡縮
		AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.8f, 0.8f, 0.8f));
		AssetManager::Effect()->SetRotation(hEffect, D3DXVECTOR3(D3DXToRadian(-90.f), 0.f, 0.f));
		AssetManager::Effect()->SetLocation(hEffect, D3DXVECTOR3(0.f, 1.f, 1.f));
	}

	//Effect制御
	if (GetAsyncKeyState('T') & 0x0001)
	{
		AssetManager::Effect()->Stop(hEffect);
	}

}