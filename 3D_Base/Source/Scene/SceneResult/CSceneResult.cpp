#include "Scene/SceneResult/CSceneResult.h"

CSceneResult::CSceneResult(CInput& input)
	: m_pSpriteResultImg	( nullptr )
	, m_pPlayer				()
	, m_Input				( input )
{
	Create();
	LoadData();
}

CSceneResult::~CSceneResult()
{
}

HRESULT CSceneResult::Create()
{
	m_pSpriteResultImg = std::make_unique<CSprite2D>();

	return S_OK;
}

HRESULT CSceneResult::LoadData()
{
	CSprite2D::SPRITE_STATE Title =
	{ WND_W,WND_H,WND_W,WND_H,WND_W,WND_H };

	m_pSpriteResultImg->Init(_T("Data\\Texture\\Result_kari.png"), Title);

	return S_OK;
}


void CSceneResult::Update()
{
	m_Input.Update();

	if (m_Input.IsDown(Action::Decide, true))
	{
		SetNextScene(Title);
	}
}


void CSceneResult::Draw()
{
	m_pSpriteResultImg->Render();
}

void CSceneResult::Destroy()
{
}