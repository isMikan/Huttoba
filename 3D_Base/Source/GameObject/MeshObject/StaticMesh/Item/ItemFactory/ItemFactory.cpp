#include "stdafx.h"
#include "ItemFactory.h"
#include "Item/ItemBase.h"

ItemFactory::ItemFactory()
{
}

ItemFactory::~ItemFactory()
{
}


//--------------------------------------------------------------------------------------------------------------

void ItemFactory::RegisterItem(const ItemID itemId, const Createfunc& creator)
{
	//Ç‹Çæìoò^Ç≥ÇÍÇƒÇ¢Ç»Ç¢èÍçáÇÃÇ›ìoò^Ç∑ÇÈ
	if(!m_Registry.count(itemId))
	{
		m_Registry[itemId] = creator;
	}
}


//--------------------------------------------------------------------------------------------------------------

std::unique_ptr<ItemBase> ItemFactory::CreateItem(const ItemID itemId)
{
	//ìoò^Ç≥ÇÍÇƒÇ¢ÇÈÉAÉCÉeÉÄÇê∂ê¨Ç∑ÇÈ
	if (m_Registry.count(itemId))
	{
		return m_Registry[itemId]();
	}

	return nullptr;
}

//--------------------------------------------------------------------------------------------------------------

