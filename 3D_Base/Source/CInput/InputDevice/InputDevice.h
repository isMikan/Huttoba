#pragma once
#include "../CXInput/CXInput.h"

enum class Action
{
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,

	Attack,
	PickUp,

	Max
};

//入力デバイス.
enum class InputDevice
{
	Keyboard,
	GamePad,

	Max
};

//アクションに対する入力の割り当て情報.
struct InputBinding
{
	InputDevice device = InputDevice::Max;	//入力デバイス(指定しないとMax).

	union Code		//中身の変数をどれか一つだけ持てる(入力デバイスに合わせて変化させる).
	{
		CXInput::KEY Gamepad;		//コントローラーのボタン.
		int Keyboard;		//キーボードのキー.

		//初期化.
		Code() :Keyboard(0) {}
	} code;	//Code型の変数code.

	//初期化時にCXInputのKEYを入れるとコントローラーのデバイスの初期化になる.
	InputBinding(InputDevice dev, CXInput::KEY key)
		:device(dev) {
		code.Gamepad = key;
	}

	//初期化時にキーを入れるとキーボードのデバイスの初期化になる.
	InputBinding(InputDevice dev, int key)
		:device(dev) {
		code.Keyboard = key;
	}
};
