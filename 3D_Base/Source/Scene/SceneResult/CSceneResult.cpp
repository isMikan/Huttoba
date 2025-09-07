#include "Scene/SceneResult/CSceneResult.h"

CSceneResult::CSceneResult(CInputManager& input)
	: m_Action				()

	, m_pSpriteResultImg	( nullptr )
	, m_pSpriteSelector		( nullptr )

	//, m_pPlayer				()

	, m_InputManager		( input )

	, m_SelectorPos			()

	, m_SelectorNumber		( 0 )

	, isHeldUp				(false)
	, isHeldDown			(false)

	, cnt					( 0 )
{
	Create();
	LoadData();
	SetSelectorPos();
}

CSceneResult::~CSceneResult()
{
}

HRESULT CSceneResult::Create()
{
	m_pSpriteResultImg = std::make_unique<CUIObject>();
	m_pSpriteSelector = std::make_unique<CUIObject>();

	return S_OK;
}

HRESULT CSceneResult::LoadData()
{
	m_pSpriteResultImg->AttachSprite(AssetManager::Sprite(Sprite2DList::Result));
	m_pSpriteSelector->AttachSprite(AssetManager::Sprite(Sprite2DList::Selector));

	return S_OK;
}


void CSceneResult::Update()
{
	m_InputManager.Update();

	MoveSelector();

	SelectorControl();

	//if (m_InputManager.GetInput(0).IsDown(Action::Decide, true))
	//{
	//	SetNextScene(Title);
	//}

	//関数を入れる
	m_Action =
	{
		//ラムダ式で関数にしてm_Actionの中に入れている(SetNextScene(Standby);ではだめ).
		//画面に表示される選択肢の文字と同じ順番に処理を入れていく
		[this]() {SetNextScene(GameMain);},
		[this]() {SetNextScene(Standby);},
		[this]() {SetNextScene(Title);}
	};

	if (m_InputManager.GetInput(0).IsDown(Action::Decide))
	{
		//選択中の番号で処理される関数が変わる.
		m_Action[m_SelectorNumber]();
	}
}


void CSceneResult::Draw()
{
	m_pSpriteSelector->Draw();
	m_pSpriteResultImg->Draw();
}

void CSceneResult::Destroy()
{
}

void CSceneResult::SetSelectorPos()
{
	m_SelectorPos.push_back(D3DXVECTOR3(790, 415, 0));
	m_SelectorPos.push_back(D3DXVECTOR3(750, 510, 0));
	m_SelectorPos.push_back(D3DXVECTOR3(770, 590, 0));
}

void CSceneResult::MoveSelector()
{
	if (m_InputManager.GetInput(0).IsDown(Action::NavigateUp)/* || 0 < m_InputManager.GetInput(0).GetLeftSthikY()*/)
	{
		if (m_SelectorNumber > 0)
			m_SelectorNumber--;
	}
	if (m_InputManager.GetInput(0).IsDown(Action::NavigateDown)/* || 0 > m_InputManager.GetInput(0).GetLeftSthikY()*/)
	{
		if (m_SelectorNumber < m_SelectorPos.size() - 1)
			m_SelectorNumber++;
	}

	m_pSpriteSelector->SetPosition(m_SelectorPos[m_SelectorNumber]);
}

void CSceneResult::SelectorControl()
{
	float stickY = m_InputManager.GetInput(0).GetLeftSthikY();

	// パラメータ
	const float threshold = 0.5f; // 入力と判定するスティックの倒し量
	const float initialDelay = 0.2f; // 最初のディレイ（秒）
	const float repeatInterval = 0.1f; // リピート間隔（秒）
	const float dt = 1.0f / 60.0f; // 固定FPSならこれでOK

	static float holdTimerUp = 0.0f;
	static float holdTimerDown = 0.0f;

	// 上方向
	if (stickY > threshold) {
		if (!isHeldUp) {
			if (m_SelectorNumber > 0)
				m_SelectorNumber--;
			isHeldUp = true;
			holdTimerUp = initialDelay;
		}
		else {
			holdTimerUp -= dt;
			if (holdTimerUp <= 0.0f) {
				if (m_SelectorNumber > 0)
					m_SelectorNumber--;
				holdTimerUp = repeatInterval;
			}
		}
	}
	else {
		isHeldUp = false;
	}

	// 下方向
	if (stickY < -threshold) {
		if (!isHeldDown) {
			if (m_SelectorNumber < m_SelectorPos.size() - 1)
				m_SelectorNumber++;
			isHeldDown = true;
			holdTimerDown = initialDelay;
		}
		else {
			holdTimerDown -= dt;
			if (holdTimerDown <= 0.0f) {
				if (m_SelectorNumber < m_SelectorPos.size() - 1)
					m_SelectorNumber++;
				holdTimerDown = repeatInterval;
			}
		}
	}
	else {
		isHeldDown = false;
	}
}