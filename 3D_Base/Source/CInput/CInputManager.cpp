#include "CInputManager.h"
#include <stdexcept>

CInputManager::CInputManager()
	: m_pInputs	()
{
	for (DWORD i = 0; i < 4; ++i)
		m_pInputs[i] = std::make_unique<CInput>(i);
}

CInputManager::~CInputManager()
{
}

void CInputManager::Update()
{
	for (auto& input : m_pInputs) 
	{
		input->Update();
	}
}

void CInputManager::BindKey(Action action, const InputBinding& binding)
{
	for (int i = 0;i < 4;i++)
	{
		m_pInputs[i]->BindKey(action, binding);
	}
}

void CInputManager::BindKey(Action action, const InputBinding& binding, int i)
{
	m_pInputs[i]->BindKey(action, binding);
}

void CInputManager::IsDown(Action action)
{
	for (auto& i : m_pInputs)
	{
		i->IsDown(action);
	}
}

void CInputManager::IsDown(Action action, int i)
{
	m_pInputs[i]->IsDown(action);
}

void CInputManager::IsUp(Action action)
{
	for (auto& i : m_pInputs)
	{
		i->IsUp(action);
	}
}

void CInputManager::IsUp(Action action, int i)
{
	m_pInputs[i]->IsUp(action);
}

void CInputManager::IsRepeat(Action action)
{
	for (auto& i : m_pInputs)
	{
		i->IsRepeat(action);
	}
}

void CInputManager::IsRepeat(Action action, int i)
{
	m_pInputs[i]->IsRepeat(action);
}
