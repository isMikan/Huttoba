#include "CInputManager.h"
#include <stdexcept>

CInputManager::CInputManager()
	: m_Inputs				()
	, m_PlayerSlot			()
{
	InitializeInput();
}

CInputManager::~CInputManager()
{
}

void CInputManager::Update()
{
	for (int i = 0;i < PlayerMax;i++)
	{
		if (m_Inputs[i])
		{
			m_Inputs[i]->Update();
		}
	}
}

void CInputManager::JoinPlayer(int inputIndex)
{
	if (inputIndex < 0 || inputIndex > PlayerMax)
		return;

	if (m_PlayerSlot[inputIndex].state == PlayerSlotState::None
		|| m_PlayerSlot[inputIndex].state == PlayerSlotState::COM)
	{
		m_PlayerSlot[inputIndex].state = PlayerSlotState::Player;

		m_PlayerSlot[inputIndex].inputIndex = inputIndex;
	}
	else if(m_PlayerSlot[inputIndex].state == PlayerSlotState::Player)
	{
		m_PlayerSlot[inputIndex].state = PlayerSlotState::COM;

		m_PlayerSlot[inputIndex].inputIndex = -1;
	}
}

CInput& CInputManager::GetPlayerIndex(int playerIndex)
{
	int inputIndex = m_PlayerSlot[playerIndex].inputIndex;

	if (inputIndex < 0 || inputIndex > PlayerMax)
	{
		//範囲外の数値が入るとエラーで終了させる.
		throw std::out_of_range("無効なプレイヤー番号.");
	}
	return *m_Inputs[inputIndex];
}

//m_Inputの作成.
void CInputManager::InitializeInput()
{
	for (int i = 0;i < PlayerMax;i++)
	{
		m_Inputs[i] = std::make_unique<CInput>(i);
		m_PlayerSlot[i].state = PlayerSlotState::None;
		m_PlayerSlot[i].inputIndex = -1;	//-1で未設定.
	}
}
