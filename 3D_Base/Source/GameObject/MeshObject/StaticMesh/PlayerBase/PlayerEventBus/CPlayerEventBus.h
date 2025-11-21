#pragma once

template<typename T>
class CPlayerEventBus
{
public:
	CPlayerEventBus() {};
	~CPlayerEventBus() {};

//======================================================================
// 	   外部で呼び出す関数.
//======================================================================
	//引数である T*（ = IItemObserver::State）が条件で呼び出す関数.
	using Callback = std::function<void(T*)>;

	//登録.
	void Subscribe(Callback callback)
	{
		//配列に追加.
		m_EventMap.push_back(callback);
	}

	//通知.
	void Publish(T* data = nullptr)
	{
		for (auto& callback : m_EventMap)
		{
			//状態を渡して処理する.
			callback(data);
		}
	}
	
private:
//======================================================================
// 	   内部で使用する変数.
//======================================================================
	std::vector<Callback>	m_EventMap;	//イベント発生時に通知する.

};