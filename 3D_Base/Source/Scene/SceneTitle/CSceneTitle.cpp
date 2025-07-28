#include "CSceneTitle.h"

CSceneTitle::CSceneTitle()
{
}

CSceneTitle::~CSceneTitle()
{
}

HRESULT CSceneTitle::Create()
{
	return S_OK;
}

HRESULT CSceneTitle::LoadData()
{
	return S_OK;
}

void CSceneTitle::Update()
{
	if (GetAsyncKeyState('Z') & 0x0001)
	{
		SetNextScene(GameMain);
	}
}

void CSceneTitle::Draw()
{
}

void CSceneTitle::Destroy()
{
}
