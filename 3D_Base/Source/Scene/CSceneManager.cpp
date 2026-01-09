#include "CSceneManager.h"

CSceneManager::CSceneManager(HWND hWnd)
	: m_pScene			( nullptr )

	, m_pDx11			()
	, m_hWnd			( hWnd )
	 
	, m_pSky			()
	, m_SpawnItemData	()
{
	m_pDx11 = CDirectX11::GetInstance();

	Create();
}

CSceneManager::~CSceneManager()
{
}

HRESULT CSceneManager::Create()
{
	m_pScene = std::make_unique<CSceneTitle>(m_hWnd);

	//空クラスのインスタンス作成
	m_pSky = std::make_unique<CSky>();

	//衝突形状実装
	CollisionManager::GetInstance()->RegisterStrategy();

	SetInputBInding();

	return S_OK;
}

HRESULT CSceneManager::LoadData()
{

	return S_OK;
}

void CSceneManager::Update()
{
	CInputManager::Update();
	CTimeManager::Update();

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
	m_pDx11->SetDepth(false);
	//空の描画.
	m_pSky->Draw();
	m_pDx11->SetDepth(true);

	m_pScene->Draw();
}

void CSceneManager::Destroy()
{
}

void CSceneManager::CreateScene(int Scene)
{
	//前のシーンを削除
	m_pScene->Destroy();

	CTimeManager::Resume();
	CTimeManager::Reset();
	CFadeManager::Reset(255.f);

	//シーン作成
	switch (Scene)
	{
	case Title:		m_pScene = std::make_unique<CSceneTitle>	(m_hWnd);					break;
	case Standby:	m_pScene = std::make_unique<CSceneStandby>	(m_SpawnItemData);			break;
	case GameMain:  m_pScene = std::make_unique<CSceneGameMain>	(m_hWnd, m_SpawnItemData);	break;
	case Result:	m_pScene = std::make_unique<CSceneResult>();							break;
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
	CInputManager::BindKey(Action::Decide, InputBinding(InputDevice::GamePad, CXInput::B), 0);
	CInputManager::BindKey(Action::Decide, InputBinding(InputDevice::Keyboard, 'Z'), 0);

	for (int i = 0;i < 4;i++)
	{
		CInputManager::BindKey(Action::Switch, InputBinding(InputDevice::GamePad, CXInput::A), i);
		CInputManager::BindKey(Action::Switch, InputBinding(InputDevice::Keyboard, 'X'), i);
	}

	CInputManager::BindKey(Action::NavigateUp, InputBinding(InputDevice::GamePad, CXInput::UP), 0);
	CInputManager::BindKey(Action::NavigateUp, InputBinding(InputDevice::Keyboard, VK_UP), 0);

	CInputManager::BindKey(Action::NavigateDown, InputBinding(InputDevice::GamePad, CXInput::DOWN), 0);
	CInputManager::BindKey(Action::NavigateDown, InputBinding(InputDevice::Keyboard, VK_DOWN), 0);
}
