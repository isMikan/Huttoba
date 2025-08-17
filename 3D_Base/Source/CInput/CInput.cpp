#include "CInput.h"
#include "XInput/CXInput.h"

CInput::CInput(DWORD playerIndex)
	: m_pXInput			()
	, m_PlayerIndex		()
	, m_SthikDeadZone	(0.2f)
{
	m_pXInput = std::make_unique<CXInput>(playerIndex);

	//mapに全てのアクションをfalseで入れる(ゲーム開始時には何の操作もさせないため).
	for (int i = 0;i < static_cast<int>(Action::Max);i++)
	{
		Action act = static_cast<Action>(i);

		m_InputState[act] = false;
		m_OldInputState[act] = false;
	}
}

CInput::~CInput()
{
}

void CInput::Update()
{
	m_pXInput->Update();

	for (const auto& table : m_InputTable)
	{
		//現在のアクションを記録.
		Action action = table.first;

		//押したかどうかを記録.
		bool isPush = false;

		//それぞれのデバイスの入力に合わせて押したかどうかを判定.
		for (const auto& binding : table.second)
		{
			switch (binding.device)
			{
			case InputDevice::GamePad:
				if (m_pXInput->IsDown(binding.code.Gamepad))
					isPush = true;
				break;
			case InputDevice::Keyboard:
				if (GetAsyncKeyState(binding.code.Keyboard) & 0x8000)
					isPush = true;
				break;
			default:
				break;
			}
		}

		m_OldInputState[action] = m_InputState[action];	//前フレームの入力を記録.
		m_InputState[action] = isPush;					//今フレームの入力を記録.
	}
}

//押した(第二引数をtrueにすると押した瞬間をとる何も入れないとfalse).
bool CInput::IsDown(Action action, bool just)
{
	if (just)
		return m_InputState.at(action) && !m_OldInputState.at(action);
	return m_InputState.at(action);
}

//離した.
bool CInput::IsUp(Action action)
{
	return !m_InputState.at(action) && m_OldInputState.at(action);
}

//押し続けている.
bool CInput::IsRepeat(Action action)
{
	return m_InputState.at(action) && m_OldInputState.at(action);
}

void CInput::BindKey(Action action, const InputBinding& binding)
{
	m_InputTable[action].push_back(binding);
}

float CInput::GetLeftSthikX()
{
	return SthikNormalize(m_pXInput->GetLThumbX());
}

float CInput::GetLeftSthikY()
{
	return SthikNormalize(m_pXInput->GetLThumbY());
}

float CInput::GetRightSthikX()
{
	return SthikNormalize(m_pXInput->GetRThumbX());
}

float CInput::GetRightSthikY()
{
	return SthikNormalize(m_pXInput->GetRThumbY());
}

float CInput::SthikNormalize(SHORT sthik)
{
	float f = static_cast<float>(sthik) / CXInput::THUMB_MAX;

	return SthikDeadZone(f);
}

float CInput::SthikDeadZone(float sthik)
{
	//abs(絶対値)がデッドゾーンより大きいなら通る.
	if (abs(sthik) > m_SthikDeadZone)
	{
		return sthik;
	}
	return 0.0f;
}
