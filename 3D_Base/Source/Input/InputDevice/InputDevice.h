#pragma once
#include "../XInput/CXInput.h"

enum class Action
{
	MoveUp,			//上移動.
	MoveDown,		//下移動.
	MoveLeft,		//左移動.
	MoveRight,		//右移動.

	Have,			//所持.
	Attack,			//攻撃.
	HaeAttack,			//攻撃.
	ToggleItem,		//アイテムの切り替え.

	Decide,			//決定.
	Switch,			//準備切り替え.
	NavigateUp,		//選択肢を上に移動.
	NavigateDown,	//選択肢を下に移動.

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
		int Keyboard;				//キーボードのキー.

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
