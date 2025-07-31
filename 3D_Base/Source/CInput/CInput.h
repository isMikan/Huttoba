#pragma once
#include <Windows.h>
#include <map>
#include <vector>

#include "CXInput/CXInput.h"
#include "InputDevice/InputDevice.h"

class CInput
{
public:
	
public:
	CInput(DWORD playerIndex);
	~CInput();

	void Update();

	//押した(第二引数をtrueにすると押した瞬間をとる何も入れないとfalse).
	bool IsDown(Action action, bool just = false);
	//離した.
	bool IsUp(Action action);
	//押し続けている.
	bool IsRepeat(Action action);

	//キーバインドで使用(使用例).
	//m_Input->BindKey(Action::Attack, InputBinding(InputDevice::GamePad, CXInput::B));
	//m_Input->BindKey(Action::Attack, InputBinding(InputDevice::Keyboard, 'Q'));
	void BindKey(Action action, const InputBinding& binding);

	//正規化したスティックの値を渡す.
	float ThumbNormalize(SHORT thumb);

	//左スティックのX軸を取得.
	float GetLeftSthikX() { return ThumbNormalize(m_pXInput->GetLThumbX()); }
	//左スティックのY軸を取得.
	float GetLeftSthikY() { return ThumbNormalize(m_pXInput->GetLThumbY()); }
	//右スティックのX軸を取得.
	float GetRightSthikX() { return ThumbNormalize(m_pXInput->GetRThumbX()); }
	//右スティックのY軸を取得.
	float GetRightSthikY() { return ThumbNormalize(m_pXInput->GetRThumbY()); }

private:
	CXInput*	m_pXInput;
	DWORD		m_PlayerIndex;

	//対応するアクションに入力デバイスと入力コードを複数登録できる.
	std::map<Action, std::vector<InputBinding> >	m_InputTable;
	//今フレームでアクションに対応するボタンかキーを押されていたらtrue.
	std::map<Action, bool>	m_InputState;
	//前フレームでアクションに対応するボタンかキーを押されていたらtrue.
	std::map<Action, bool>	m_OldInputState;

	float				m_ThumbDeadZone;//スティックのデッドゾーン.
};