#include "stdafx.h"
#include "ItemFactory.h"

ItemFactory::ItemFactory()
{
}

ItemFactory::~ItemFactory()
{
}

//-----------------------------------------------------------------------

void ItemFactory::RegisterItem(std::string itemId, const Createfunc creator)
{
	//‚Ü‚¾“o˜^‚³‚ê‚Ä‚¢‚È‚¢ê‡‚Ì‚İ“o˜^‚·‚é
	if(!m_Registry.count(itemId));
	{
		m_Registry[itemId] = creator;
	}
}

//-----------------------------------------------------------------------

std::shared_ptr<ItemBase> ItemFactory::CreateItem(const std::string itemId)
{
	//“o˜^‚³‚ê‚Ä‚¢‚éƒAƒCƒeƒ€‚ğ¶¬‚·‚é
	if (m_Registry.count(itemId))
	{
		return m_Registry[itemId]();
	}

	return nullptr;
}

//-----------------------------------------------------------------------

