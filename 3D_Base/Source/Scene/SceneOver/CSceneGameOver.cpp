#include "CSceneGameOver.h"

CSceneGameOver::CSceneGameOver()
{
}

CSceneGameOver::~CSceneGameOver()
{
}

HRESULT CSceneGameOver::Create()
{
	return S_OK;
}

HRESULT CSceneGameOver::LoadData()
{
	return S_OK;
}

void CSceneGameOver::Update()
{

	if (GetAsyncKeyState('Z') & 0x0001)
	{
		SetNextScene(Ending);
	}
}

void CSceneGameOver::Draw()
{
}

void CSceneGameOver::Destroy()
{
}
