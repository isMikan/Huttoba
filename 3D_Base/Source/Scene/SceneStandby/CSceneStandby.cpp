#include "CSceneStandby.h"

CSceneStandby::CSceneStandby(CInput& input)
	: m_pSpriteStandbyImg	( nullptr )
	, m_pCamera				( nullptr )
	, m_pPlayer				()
	, m_Input				( input )
{
	Create();
	LoadData();
	InitializePlayers();
}

CSceneStandby::~CSceneStandby()
{
}

HRESULT CSceneStandby::Create()
{
	m_pSpriteStandbyImg = std::make_unique<CSprite2D>();
	m_pCamera = std::make_unique<CCamera>();

	return S_OK;
}

HRESULT CSceneStandby::LoadData()
{
	CSprite2D::SPRITE_STATE Title =
	{ WND_W,WND_H,WND_W,WND_H,WND_W,WND_H };

	m_pSpriteStandbyImg->Init(_T("Data\\Texture\\Standby_kari.png"), Title);

	return S_OK;
}

void CSceneStandby::Update()
{
	m_Input.Update();

	if (m_Input.IsDown(Action::Decide, true))
	{
		SetNextScene(GameMain);
	}
}

void CSceneStandby::Draw()
{
	m_pCamera->Update();	//値をとる前に処理したいのでここに入れた(Updateに入れるべきかも.

	D3DXMATRIX	mView = m_pCamera->GetView();
	D3DXMATRIX	mProj = m_pCamera->GetProj();
	LIGHT		light = m_pCamera->GetLight();
	CAMERA		camera = m_pCamera->GetCamera();

	//m_pSpriteStandbyImg->Render();	//一番前に表示されるので文字などを表示させたい際は要検証.

	for (int i = 0;i < 4;i++)
	{
		m_pPlayer[i]->Draw(mView, mProj, light, camera);
	}
}

void CSceneStandby::Destroy()
{
}

void CSceneStandby::InitializePlayers()
{
	//プレイヤーの人数だけ処理(マジックナンバーなのを後で変える).
	for (int i = 0;i < 4;i++)
	{
		m_pPlayer.push_back(std::make_unique<CPlayer>());
		m_pPlayer[i]->AttachMesh(AssetManager::Mesh(StaticMeshList::Player));
		m_pPlayer[i]->SetPosition(-12.f + (9.f * i), 1.f, 6.f);
	}
}
