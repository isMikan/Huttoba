#include "Scene/SceneResult/CSceneResult.h"

CSceneResult::CSceneResult()
{
}

CSceneResult::~CSceneResult()
{
}

HRESULT CSceneResult::Create()
{
	return S_OK;
}

HRESULT CSceneResult::LoadData()
{
	return S_OK;
}


void CSceneResult::Update()
{
	if (GetAsyncKeyState('Z') & 0x0001)
	{
		SetNextScene(Title);
	}
}


void CSceneResult::Draw()
{
}

void CSceneResult::Destroy()
{
}
