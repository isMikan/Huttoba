#include "CSceneManager.h"

CSceneManager::CSceneManager(HWND hWnd)
	: m_pScene		( nullptr )
	, m_pInput		( nullptr )
	, m_hWnd		( hWnd )
{
	Create();
}

CSceneManager::~CSceneManager()
{
}

HRESULT CSceneManager::Create()
{
	m_pScene = std::make_unique<CSceneTitle>(m_hWnd, *m_pInput);
	m_pInput = std::make_unique<CInput>(0);

	SetInputBInding();

	return S_OK;
}

HRESULT CSceneManager::LoadData()
{

	return S_OK;
}

void CSceneManager::Update()
{
	//シーンが変更されてるか？
	if (m_pScene->GetChangetScene() == true)
	{
		//次のシーンを作成
		CreateScene(m_pScene->GetNextScene());
	}

	m_pScene->Update();
}

void CSceneManager::Draw()
{
	m_pScene->Draw();
}

void CSceneManager::Destroy()
{
}

void CSceneManager::CreateScene(int Scene)
{
	//前のシーンを削除
	m_pScene->Destroy();

	//シーン作成
	switch (Scene)
	{
	case Title:		m_pScene = std::make_unique<CSceneTitle>	(m_hWnd, *m_pInput);break;
	case Standby:	m_pScene = std::make_unique<CSceneStandby>	(*m_pInput);		break;
	case GameMain:  m_pScene = std::make_unique<CSceneGameMain>	(m_hWnd);			break;
	case Result:	m_pScene = std::make_unique<CSceneResult>	(*m_pInput);		break;
	default: 
		//終了
		return;
		break;
	}
	//シーン変更確認用フラッグをfalseに
	m_pScene->ChangeSceneFlagFalse();
}

void CSceneManager::SetInputBInding()
{
	m_pInput->BindKey(Action::Decide, InputBinding(InputDevice::GamePad, CXInput::B));
	m_pInput->BindKey(Action::Decide, InputBinding(InputDevice::Keyboard, 'Z'));


	m_pInput->BindKey(Action::NavigateUp, InputBinding(InputDevice::GamePad, CXInput::UP));
	m_pInput->BindKey(Action::NavigateUp, InputBinding(InputDevice::Keyboard, VK_UP));

	m_pInput->BindKey(Action::NavigateDown, InputBinding(InputDevice::GamePad, CXInput::DOWN));
	m_pInput->BindKey(Action::NavigateDown, InputBinding(InputDevice::Keyboard, VK_DOWN));
}
