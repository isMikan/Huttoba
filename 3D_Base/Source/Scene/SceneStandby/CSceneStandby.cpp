#include "CSceneStandby.h"

CSceneStandby::CSceneStandby(CInput& input)
	: m_pSpriteStandbyImg	( nullptr )
	, m_Input				( input )
{
	Create();
	LoadData();
}

CSceneStandby::~CSceneStandby()
{
}

HRESULT CSceneStandby::Create()
{
	m_pSpriteStandbyImg = std::make_unique<CSprite2D>();

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
	m_pSpriteStandbyImg->Render();
}

void CSceneStandby::Destroy()
{
}