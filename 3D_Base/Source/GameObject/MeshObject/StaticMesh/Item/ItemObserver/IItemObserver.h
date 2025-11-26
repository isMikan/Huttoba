#pragma once

//ゲージのために必要だったので勝手に作りました.		制作者　[甲把]
class IItemObserver
{
public:
	//アイテムの状態.
	enum class State
	{
		None = -1,
		Spawn,
		OnGround,
		Have,
		Use,
		Throw,
		Destroy,
	};

public:
	virtual void ItemState(State state) = 0;	//状態変化時に一度だけ処理.

};

