#pragma once

template<typename T>
class CPlayerEventBus
{
public:
	CPlayerEventBus() {};
	~CPlayerEventBus() {};

	using Callback = std::function<void(T*)>;

	//“o˜^.
	void Subscribe(Callback callback)
	{
		m_EventMap.push_back(callback);
	}

	//’Ê’m.
	void Publish(T* data = nullptr)
	{
		for (auto& callback : m_EventMap)
		{
			callback(data);
		}
	}

private:
	std::vector<Callback>	m_EventMap;

};