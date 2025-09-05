#include "CSceneManager.h"

CSceneManager::CSceneManager(HWND hWnd)
	: m_pScene			( nullptr )
	, m_pInputManager	( CInputManager::Instance() )
	, m_hWnd			( hWnd )
{
	Create();
}

CSceneManager::~CSceneManager()
{
}

HRESULT CSceneManager::Create()
{
	//m_pInputManager = std::make_unique<CInputManager>();
	m_pScene = std::make_unique<CSceneTitle>(m_hWnd, m_pInputManager);

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
	case Title:		m_pScene = std::make_unique<CSceneTitle>	(m_hWnd, m_pInputManager);	break;
	case Standby:	m_pScene = std::make_unique<CSceneStandby>	(m_pInputManager);			break;
	case GameMain:  m_pScene = std::make_unique<CSceneGameMain>	(m_hWnd, m_pInputManager);	break;
	case Result:	m_pScene = std::make_unique<CSceneResult>	(m_pInputManager);			break;
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
	//m_pInputManager.BindKey(Action::Decide, InputBinding(InputDevice::GamePad, CXInput::B));
	//m_pInputManager.BindKey(Action::Decide, InputBinding(InputDevice::Keyboard, 'Z'));

	//m_pInputManager.BindKey(Action::Switch, InputBinding(InputDevice::GamePad, CXInput::A));
	//m_pInputManager.BindKey(Action::Switch, InputBinding(InputDevice::Keyboard, 'X'));
	//			   
	//m_pInputManager.BindKey(Action::NavigateUp, InputBinding(InputDevice::GamePad, CXInput::UP));
	//m_pInputManager.BindKey(Action::NavigateUp, InputBinding(InputDevice::Keyboard, VK_UP));
	//			   
	//m_pInputManager.BindKey(Action::NavigateDown, InputBinding(InputDevice::GamePad, CXInput::DOWN));
	//m_pInputManager.BindKey(Action::NavigateDown, InputBinding(InputDevice::Keyboard, VK_DOWN));

	CInputManager::Instance().BindKey(Action::Decide, InputBinding(InputDevice::GamePad, CXInput::B));
	CInputManager::Instance().BindKey(Action::Decide, InputBinding(InputDevice::Keyboard, 'Z'));
				   
	CInputManager::Instance().BindKey(Action::Switch, InputBinding(InputDevice::GamePad, CXInput::A));
	CInputManager::Instance().BindKey(Action::Switch, InputBinding(InputDevice::Keyboard, 'X'));

	CInputManager::Instance().BindKey(Action::NavigateUp, InputBinding(InputDevice::GamePad, CXInput::UP));
	CInputManager::Instance().BindKey(Action::NavigateUp, InputBinding(InputDevice::Keyboard, VK_UP));
				
	CInputManager::Instance().BindKey(Action::NavigateDown, InputBinding(InputDevice::GamePad, CXInput::DOWN));
	CInputManager::Instance().BindKey(Action::NavigateDown, InputBinding(InputDevice::Keyboard, VK_DOWN));
}
