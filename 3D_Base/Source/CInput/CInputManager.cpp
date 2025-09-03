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

void CInputManager::Update()
{
	for (auto& input : m_pInputs) 
	{
		input->Update();
	}
}
