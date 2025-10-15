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

void CInputManager::BindKey(Action action, const InputBinding& binding, int index)
{
	m_pInputs[index]->BindKey(action, binding);
}

bool CInputManager::IsDown(Action action, int index)
{
	return m_pInputs[index]->IsDown(action);
}

bool CInputManager::IsUp(Action action, int index)
{
	return m_pInputs[index]->IsUp(action);
}

bool CInputManager::IsRepeat(Action action, int index)
{
	return m_pInputs[index]->IsRepeat(action);
}

float CInputManager::GetLeftSthikX(int index)
{
	return m_pInputs[index]->GetLeftSthikX();
}

float CInputManager::GetLeftSthikY(int index)
{
	return m_pInputs[index]->GetLeftSthikY();
}

float CInputManager::GetRightSthikX(int index)
{
	return m_pInputs[index]->GetRightSthikX();
}

float CInputManager::GetRightSthikY(int index)
{
	return m_pInputs[index]->GetRightSthikY();
}
