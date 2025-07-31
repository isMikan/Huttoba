#pragma once

class ItemBase;

class ItemManager
{
public:
	ItemManager();
	~ItemManager();



	HRESULT Create();
	HRESULT LoadData();
	void Init();
	void Update();
	void Draw();

private:
	std::vector<std::unique_ptr<ItemBase>> m_pItems;

};