#include "CSceneGameMain.h"
#include "Assets/Effect/CEffect.h"
#include "Assets/Sound/CSoundManager.h"

CSceneGameMain::CSceneGameMain(HWND hWnd)
	: m_hWnd(hWnd)
	, m_pDbgText(nullptr)

	, m_pCamera(nullptr)

	, m_pUIMap()

	, m_pExplosiones()

	, m_pPlayer(nullptr)
	, m_pEnemies()

	, m_pGround(nullptr)

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
	m_pCamera = new CCamera();

	//あらかじめ領域確保
	m_pExplosiones.resize(Explosion_Max);

	//デバッグテキストのインスタンス作成
	m_pDbgText = std::make_unique<CDebugText>();

	AssetManager::GetInstance()->Create();

	//各オブジェクトのインスタンス作成
	CreateUI();
	CteateExplosion();
	CreateCharactor();
	//地面クラスのインスタンス作成
	m_pGround = std::make_unique<CGround>();

	return S_OK;
}

HRESULT CSceneGameMain::LoadData()
{

	//デバッグテキストの読み込み
	if (FAILED(m_pDbgText->Init())) {
		return E_FAIL;
	}

	AssetManager::GetInstance()->LoadData();

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

	//スタティックメッシュを設定
	m_pPlayer->AttachMesh(AssetManager::Mesh(StaticMeshList::Player));

	m_pGround->AttachMesh(AssetManager::Mesh(StaticMeshList::Ground));

	AttachMeshToEnemy();

	//Pモンそれぞれの画像パターンを設定
	m_pUIMap[UIList::Beedrill]->SetPatternNo(14, 0);
	m_pUIMap[UIList::Parasect]->SetPatternNo(14, 2);
	m_pUIMap[UIList::Scyther]->SetPatternNo(10, 7);

	//Pモンそれぞれの位置を設定
	const float size = 64.f;	// 64.f
	const float pos_y = static_cast<float>(WND_H) - 64.f;
	m_pUIMap[UIList::Beedrill]->SetPosition(size * 0.f, pos_y, 0.f);
	m_pUIMap[UIList::Parasect]->SetPosition(size * 1.f, pos_y, 0.f);
	m_pUIMap[UIList::Scyther]->SetPosition(size * 2.f, pos_y, 0.f);

	//バウンディングスフィアの作成
	m_pPlayer->CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::BSphere));

	for (auto& enemyType : m_pEnemies)
	{
		for (auto& enemy : enemyType.second)
		{
			enemy->CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::BSphere));
		}
	}

	//キャラクターの初期座標を設定
	m_pPlayer->SetPosition(0.f, 1.f, 6.f);

	for (auto& enemyType : m_pEnemies)
	{
		for (auto& enemy : enemyType.second)
		{
			enemy->SetPosition(0.f, 1.f, 16.f);
		}
	}
	//エネミー複数設定

	for (auto& enemy : m_pEnemies[EnemyList::RoboA])
	{
		int i = static_cast<int>(&enemy - &m_pEnemies[EnemyList::RoboA][0]);

		enemy->AttachMesh(AssetManager::Mesh(StaticMeshList::RoboA));
		enemy->CreateBSphereForMesh(AssetManager::Mesh(StaticMeshList::BSphere));
		enemy->SetPosition(-3.f + (i * 3.f), 1.f, 10.f);
	}

	return S_OK;
}


void CSceneGameMain::Destroy()
{
}

void CSceneGameMain::Update()
{
	//BGMのループ再生
	AssetManager::Sound()->PlayLoop(CSoundManager::BGM_Bonus);

	m_pGround->Update();
	m_pPlayer->Update();

	for (auto& enemyType : m_pEnemies)
	{
		for (auto& enemy : enemyType.second)
		{
			enemy->Update();
		}
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

	//--------------------
	//	スキンメッシュ
	//--------------------

	for (auto& UI : m_pUIMap)
	{
		UI.second->Update();
	}

	//ほんとはメンバ変数で作ってあげる
	//エフェクトのインスタンスごとに必要になるハンドル
	//※3つ制御するなら3つ必要
	static ::EsHandle hEffect = -1;

	//Effect制御
	if (GetAsyncKeyState('Y') & 0x0001)
	{
		hEffect = AssetManager::Effect()->Play("Laser", m_pPlayer->GetPosition());

		//拡縮
		AssetManager::Effect()->SetScale(hEffect, D3DXVECTOR3(0.8f, 0.8f, 0.8f));
		AssetManager::Effect()->SetRotation(hEffect, D3DXVECTOR3(D3DXToRadian(-90.f), 0.f, 0.f));
		AssetManager::Effect()->SetLocation(hEffect, D3DXVECTOR3(0.f, 1.f, 1.f));
	}
	if (GetAsyncKeyState('T') & 0x0001)
	{
		AssetManager::Effect()->Stop(hEffect);
	}

	if (GetAsyncKeyState('L') & 0x0001)
	{
		SetNextScene(GameOver);
	}

}

void CSceneGameMain::Draw()
{
	m_pCamera->Update();

	CAMERA camera = m_pCamera->GetCamera();
	LIGHT light = m_pCamera->GetLight();
	D3DXMATRIX mView = m_pCamera->GetView();
	D3DXMATRIX mProj = m_pCamera->GetProj();


	m_pPlayer->Draw(mView, mProj, light, camera);

	for (auto& enemyType : m_pEnemies)
	{
		for (auto& enemy : enemyType.second)
		{
			enemy->Draw(mView, mProj, light, camera);
		}
	}

	////ボーン座標に合わせて球体を表示
	//m_pStaticMeshMap[StaticMeshList::BSphere]->SetPosition(m_ZakoBonePos);
	//m_pStaticMeshMap[StaticMeshList::BSphere]->Render(m_mView, m_mProj, m_Light, m_Camera.vPosition);

	//深度テスト無効にすることで、処理順番で描画させることができる
	m_pDx11->SetDepth(false);

	for (auto& UI : m_pUIMap)
	{
		UI.second->Draw();
	}

	//やりたいことが終わったので、深度テストを有効にしておく
	m_pDx11->SetDepth(true);

	//当たり判定の中心座標を更新する
	m_pPlayer->UpdateBSpherePos();


	for (auto& enemyType : m_pEnemies)
	{
		for (auto& enemy : enemyType.second)
		{
			enemy->UpdateBSpherePos();
		}
	}
	//プレイヤーとエネミーの当たり判定
	for (auto& enemyType : m_pEnemies)
	{
		for (auto& enemy : enemyType.second)
		{
			if (m_pPlayer->GetBSphere()->IsHit(*enemy->GetBSphere()))
			{
				SetWindowText(m_hWnd, _T("衝突しています"));
			}
			else
			{
				SetWindowText(m_hWnd, _T(""));
			}
		}
	}

	for (auto& exp : m_pExplosiones)
	{
		exp->Draw(mView, mProj);
	}

	//デバッグテキストの描画
	m_pDbgText->SetColor(0.9f, 0.6f, 0.f);	//色の設定
	m_pDbgText->Render(_T("ABCD"), 10, 100);

	//デバッグテキスト(数値入り)の描画
	m_pDbgText->SetColor(1.f, 0.f, 0.f);
	TCHAR dbgText[64];
	_stprintf_s(dbgText, _T("Float:%f, %f"), 1.f, 2.2f);
	m_pDbgText->Render(dbgText, 10, 110);

	//Effectクラス
	AssetManager::Effect()->Draw(mView, mProj, light, camera);

}

HRESULT CSceneGameMain::CreateUI()
{
	UIList UI[] =
	{
		UIList::Pmon,
		UIList::Beedrill,
		UIList::Parasect,
		UIList::Scyther,
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
	m_pPlayer = std::make_unique<CPlayer>();
	if (!m_pPlayer) return E_POINTER;

	m_pEnemies[EnemyList::RoboB].push_back(std::make_unique<CEnemy>());

	for (int i = 0; i < Enemy_Max; i++)
	{
		m_pEnemies[EnemyList::RoboA].push_back(std::make_unique<CEnemy>());
	}

	//nullチェック
	for (const auto& enemyType : m_pEnemies)
	{
		for (const auto& e : enemyType.second)
		{
			if (!e) return E_POINTER;
		}
	}

	return E_NOTIMPL;
}

void CSceneGameMain::AttachMeshToEnemy()
{
	//敵の種類分回す
	for (auto& enemyType : m_pEnemies)
	{
		//その種類の敵の数分回す
		for (auto& enemy : enemyType.second)
		{
			//種類によってメッシュ変更
			switch (enemyType.first)
			{
			case EnemyList::RoboA:

				enemy->AttachMesh(AssetManager::Mesh(StaticMeshList::RoboA));
				break;

			case EnemyList::RoboB:

				enemy->AttachMesh(AssetManager::Mesh(StaticMeshList::RoboB));
				break;

			default:
				break;
			}
		}
	}
}