#pragma once
#include <Windows.h>
#include <map>
#include <vector>

#include "XInput/CXInput.h"
#include "InputDevice/InputDevice.h"

class CInput
{
public:
	
public:
	CInput(DWORD playerIndex);
	~CInput();

	void Update();

	//押した(第二引数をfalseにすると押している時をとる).
	//押し続けているのと似ているのでfalse処理は消してもいいかも.
	bool IsDown(Action action, bool just = true);
	//離した.
	bool IsUp(Action action);
	//押し続けている.
	bool IsRepeat(Action action);

	//キーバインドで使用(使用例).
	//m_Input->BindKey(Action::Attack, InputBinding(InputDevice::GamePad, CXInput::B));
	//m_Input->BindKey(Action::Attack, InputBinding(InputDevice::Keyboard, 'Q'));
	void BindKey(Action action, const InputBinding& binding);

	//左スティックの正規化したX軸を取得.
	float GetLeftSthikX();
	//左スティックの正規化したY軸を取得.
	float GetLeftSthikY();
	//右スティックの正規化したX軸を取得.
	float GetRightSthikX();
	//右スティックの正規化したY軸を取得.
	float GetRightSthikY();

	//コントローラーが接続されているかを返す.
	bool IsConnect() { return m_pXInput->IsConnect(); }

private:
	//正規化したスティックの値を渡す.
	float SthikNormalize(SHORT sthik);

	//スティックのデッドゾーン処理.
	float SthikDeadZone(SHORT sthik);

private:
	std::unique_ptr<CXInput>	m_pXInput;
	DWORD		m_PlayerIndex;

	//対応するアクションに入力デバイスと入力コードを複数登録できる.
	std::map<Action, std::vector<InputBinding> >	m_InputTable;
	//今フレームでアクションに対応するボタンかキーを押されていたらtrue.
	std::map<Action, bool>	m_InputState;
	//前フレームでアクションに対応するボタンかキーを押されていたらtrue.
	std::map<Action, bool>	m_OldInputState;

	float				m_SthikDeadZone;	//スティックのデッドゾーン.
};