#include "CScene.h"

CScene::CScene()
	: m_NextScene	(Title)
	, ChangeScene	( true )
{
}

CScene::~CScene()
{
}

void CScene::Update()
{
}

void CScene::Draw()
{
}

HRESULT CScene::Create()
{
	return S_OK;
}

void CScene::Destroy()
{
}

