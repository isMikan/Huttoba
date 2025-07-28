#include "CSceneGameMain.h"
#include "Assets/Effect/CEffect.h"
#include "Assets/Sound/CSoundManager.h"


CSceneGameMain::CSceneGameMain(HWND hWnd)
	: m_hWnd			(hWnd)
	, m_pDbgText(nullptr)
	, m_mView()
	, m_mProj()

	, m_Camera()
	, m_Light()

	, m_pUIMap()
	, m_pStaticMeshMap()
	, m_pSkinMeshMap()

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
	//カメラ座標.
	m_Camera.vPosition = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 10.0f);

	//ライト情報
	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);	//ライト方向

	//あらかじめ領域確保
	m_pExplosiones.resize(Explosion_Max);

	//デバッグテキストのインスタンス作成
	m_pDbgText = std::make_unique<CDebugText>();

	if (FAILED(SpriteManager::GetInstance()->Create())){ return E_FAIL; }

	//各オブジェクトのインスタンス作成
	CreateStaticMesh();
	CreateUI();
	CreateSkinMesh();
	CteateExplosion();
	CreateCharactor();
	//地面クラスのインスタンス作成
	m_pGround = std::make_unique<CGround>();

	//エフェクトクラス
	CEffect::GetInstance()->Create(
		m_pDx11->GetDevice(),
		m_pDx11->GetContext());

	return S_OK;
}

HRESULT CSceneGameMain::LoadData()
{
	if (FAILED(CEffect::GetInstance()->LoadData()))
	{
		return E_FAIL;
	}

	//デバッグテキストの読み込み
	if (FAILED(m_pDbgText->Init())) {
		return E_FAIL;
	}

	if (FAILED(SpriteManager::GetInstance()->LoadData())) { return E_FAIL; }

	//スタティックメッシュの読み込み
	m_pStaticMeshMap[StaticMeshList::Fighter]	->Init(_T("Data\\Mesh\\Static\\Fighter\\Fighter.x"));
	m_pStaticMeshMap[StaticMeshList::Ground]	->Init(_T("Data\\Mesh\\Static\\Ground\\ground.x"));
	m_pStaticMeshMap[StaticMeshList::RoboA]		->Init(_T("Data\\Mesh\\Static\\Robo\\RobotA_pivot.x"));
	m_pStaticMeshMap[StaticMeshList::RoboB]		->Init(_T("Data\\Mesh\\Static\\Robo\\RobotB_pivot.x"));
	m_pStaticMeshMap[StaticMeshList::Bullet]	->Init(_T("Data\\Mesh\\Static\\Bullet\\bullet.x"));

	//バウンディングスフィア(当たり判定用)
	m_pStaticMeshMap[StaticMeshList::BSphere]->Init(_T("Data\\Collision\\Sphere.x"));

	//スキンメッシュの読み込み
	m_pSkinMeshMap[SkinMeshList::Zako]->Init(_T("Data\\Mesh\\Skin\\zako\\zako.x"));

	//爆発スプライトを設定.
	for (const auto& exp : m_pExplosiones)
	{
		exp->AttachSprite(SpriteManager::GetInstance()->GetSprite3D(Sprite3DList::Explosion));
	}

	//Pモンスプライトを設定
	for (auto& UI : m_pUIMap)
	{
		UI.second->AttachSprite(SpriteManager::GetInstance()->GetSprite2D(Sprite2DList::PMon));
	}

	//スタティックメッシュを設定
	m_pPlayer->AttachMesh(*m_pStaticMeshMap[StaticMeshList::Fighter]);
	m_pGround->AttachMesh(*m_pStaticMeshMap[StaticMeshList::Ground]);

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
	m_pPlayer->CreateBSphereForMesh(*m_pStaticMeshMap[StaticMeshList::BSphere]);

	for (auto& enemyType : m_pEnemies)
	{
		for (auto& enemy : enemyType.second)
		{
			enemy->CreateBSphereForMesh(*m_pStaticMeshMap[StaticMeshList::BSphere]);
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

		enemy->AttachMesh(*m_pStaticMeshMap[StaticMeshList::RoboA]);
		enemy->CreateBSphereForMesh(*m_pStaticMeshMap[StaticMeshList::BSphere]);
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
	CSoundManager::PlayLoop(CSoundManager::BGM_Bonus);

	//カメラ座標のデバックコマンド.
	float add_value = 0.1f;
	if (GetAsyncKeyState('W') & 0x8000) {
		m_Camera.vPosition.y += add_value;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		m_Camera.vPosition.y -= add_value;
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		m_Camera.vPosition.x -= add_value;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		m_Camera.vPosition.x += add_value;
	}
	if (GetAsyncKeyState('Q') & 0x8000) {
		m_Camera.vPosition.z += add_value;
	}
	if (GetAsyncKeyState('E') & 0x8000) {
		m_Camera.vPosition.z -= add_value;
	}

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
		hEffect = CEffect::Play("Laser", m_pPlayer->GetPosition());

		//拡縮
		CEffect::SetScale(hEffect, D3DXVECTOR3(0.8f, 0.8f, 0.8f));
		CEffect::SetRotation(hEffect, D3DXVECTOR3(D3DXToRadian(-90.f), 0.f, 0.f));
		CEffect::SetLocation(hEffect, D3DXVECTOR3(0.f, 1.f, 1.f));
	}
	if (GetAsyncKeyState('T') & 0x0001)
	{
		CEffect::Stop(hEffect);
	}

	if (GetAsyncKeyState('L') & 0x0001)
	{
		SetNextScene(GameOver);
	}

}

void CSceneGameMain::Draw()
{
	Camera();
	Projection();

	//	m_pStcMeshObj->Draw( m_mView, m_mProj, m_Light, m_Camera );

	m_pGround->Draw(m_mView, m_mProj, m_Light, m_Camera);

	m_pPlayer->Draw(m_mView, m_mProj, m_Light, m_Camera);

	for (auto& enemyType : m_pEnemies)
	{
		for (auto& enemy : enemyType.second)
		{
			enemy->Draw(m_mView, m_mProj, m_Light, m_Camera);
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
		exp->Draw(m_mView, m_mProj);
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
	CEffect::GetInstance()->Draw(m_mView, m_mProj, m_Light, m_Camera);

}


HRESULT CSceneGameMain::CreateStaticMesh()
{
	StaticMeshList MeshList[] =
	{
		StaticMeshList::Fighter,
		StaticMeshList::Ground,
		StaticMeshList::RoboA,
		StaticMeshList::RoboB,
		StaticMeshList::Bullet,
		StaticMeshList::BSphere,
	};
	for (auto& id : MeshList)
	{
		m_pStaticMeshMap[id] = std::make_unique<CStaticMesh>();
		if (!m_pStaticMeshMap[id]) E_POINTER;
	}

	return S_OK;
}

HRESULT CSceneGameMain::CreateSkinMesh()
{
	SkinMeshList skinMesh[] =
	{
		SkinMeshList::Zako
	};

	//スキンメッシュのインスタンス作成
	for (auto& id : skinMesh)
	{
		m_pSkinMeshMap[id] = std::make_unique<CSkinMesh>();
		if (!m_pSkinMeshMap[id]) return E_POINTER;
	}

	return S_OK;
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

				enemy->AttachMesh(*m_pStaticMeshMap[StaticMeshList::RoboA]);
				break;

			case EnemyList::RoboB:

				enemy->AttachMesh(*m_pStaticMeshMap[StaticMeshList::RoboB]);
				break;

			default:
				break;
			}
		}
	}
}

//カメラ関数.
//※カメラ専用のクラスを作成したら便利だよ.
void CSceneGameMain::Camera()
{
	D3DXVECTOR3 cam_pos = m_Camera.vPosition;
	D3DXVECTOR3 cam_look = m_Camera.vLook;
	D3DXVECTOR3	vUpVec(0.0f, 1.0f, 0.0f);	//上方（ベクトル）.

	//ビュー（カメラ）変換.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&cam_pos, &cam_look, &vUpVec);
}

//プロジェクション関数.
void CSceneGameMain::Projection()
{
	//y方向の視野角。数値を大きくしたら視野が狭くなる.
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));	//ラジアン値.
	//アスペクト（幅÷高さ）.
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 100.0f;

	//プロジェクション（射影）変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)プロジェクション計算結果.
		fov_y,		//視野角（FOV：Field of View）.
		aspect,		//アスペクト.
		near_z,		//近いビュー平面のz値.
		far_z);	//遠いビュー平面のz値.
}


