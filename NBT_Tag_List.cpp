#include "NBT_Tag_List.h"
#include "NBT_Buffer.h"
#include "NBT_Debug.h"

NBT_Tag_List::NBT_Tag_List(bool named) : NBT_Tag(named, TAG_List)
{
	
}

NBT_Tag_List::~NBT_Tag_List()
{
	for(auto &item: item_list)
		delete item;

	 item_list.clear();
}

bool NBT_Tag_List::decodeTag(NBT_Buffer *buff)
{
	if(!NBT_Tag::decodeTag(buff))
		return false;
	
	if(!buff->read(&itemType))
		return false;
	
	int32_t count = 0;
	
	if(!buff->readInt(&count))
		return false;
	
	NBT_Debug("List \"%s\" (%s) begin (%i)", name().c_str(), itemType < TAG_LAST_ITEM ? NBT_Tag::tagNames[itemType] : "UNKNOWN", count);
	
	
	for(int i = 0; i < count; i++)
	{
		NBT_Tag *tag = tagFromType(itemType, false);
		if(!tag)
			break;
		
		if(!tag->decodeTag(buff))
			break;
		
		item_list.push_back(tag);
	}
	
	if(count != (int32_t)item_list.size())
	{
		for(auto &item: item_list)
			delete item;
		
		NBT_Debug("premature end of list :(");
		return false;
	}
	
	NBT_Debug("List \"%s\" (%s) end (%i)", name().c_str(), itemType < TAG_LAST_ITEM ? NBT_Tag::tagNames[itemType] : "UNKNOWN", count);
	
	return true;
}
