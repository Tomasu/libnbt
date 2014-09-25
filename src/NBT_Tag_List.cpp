#include "NBT_Tag_List.h"
#include "NBT_File.h"
#include "NBT_Debug.h"

#include <sstream>

NBT_Tag_List::NBT_Tag_List(bool named) : NBT_Tag(named, TAG_List)
{
	
}

NBT_Tag_List::~NBT_Tag_List()
{
	for(auto &item: item_list)
		delete item;

	 item_list.clear();
}

bool NBT_Tag_List::read(NBT_File *fh)
{
	if(!fh->read(&itemType))
		return false;
	
	int32_t count = 0;
	
	if(!fh->read(&count))
		return false;
	
	//NBT_Debug("List \"%s\" (%s) begin (%i)", name().c_str(), itemType < TAG_LAST_ITEM ? NBT_Tag::tagNames[itemType] : "UNKNOWN", count);
	
	for(int i = 0; i < count; i++)
	{
		NBT_Tag *tag = tagFromType(itemType, false);
		if(!tag)
		{
			NBT_Warn("failed to create tag for tag list");
			delete tag;
			break;
		}
		
		if(!tag->read(fh))
		{
			NBT_Warn("failed to read tag from tag list");
			delete tag;
			break;
		}
		
		tag->setRow(i);
		tag->setParent(this);
		item_list.push_back(tag);
	}
	
	if(count != (int32_t)item_list.size())
	{
		for(auto &item: item_list)
			delete item;
		
		NBT_Debug("premature end of list :(");
		return false;
	}
	
	//NBT_Debug("List \"%s\" (%s) end (%i)", name().c_str(), itemType < TAG_LAST_ITEM ? NBT_Tag::tagNames[itemType] : "UNKNOWN", count);
	
	return true;
}

bool NBT_Tag_List::write(NBT_File *fh)
{
	if(!fh->write(itemType))
	{
		NBT_Error("failed to write list item type to buffer");
		return false;
	}
	
	if(!fh->write((uint32_t)item_list.size()))
	{
		NBT_Error("failed to write list size to buffer");
		return false;
	}
	
	for(auto &item: item_list)
	{
		if(!item->write(fh))
		{
			NBT_Error("failed to encode tag to buffer");
			return false;
		}
	}
	
	return true;
}

std::string NBT_Tag_List::serialize()
{
   std::stringstream str;
   
   if(named())
      str << "List(" << name() << "; " << tagNames[itemType]+4 << ") {\n";
   else
      str << "List(" << this << "; " << tagNames[itemType]+4 << ") {\n";
 
   for(auto &item: item_list)
   {
      str << item->serialize() << "; ";
   }
   str << "}\n";
   return str.str();
}

void NBT_Tag_List::setItemAt(uint32_t i, NBT_Tag *t)
{
	if(checkType(t) && i < item_list.size())
		item_list[i] = t;
}

void NBT_Tag_List::insertItem(uint32_t i, NBT_Tag *t)
{
	if(checkType(t) && i <= item_list.size())
		item_list.insert(item_list.begin()+i, t);
}

void NBT_Tag_List::addItem(NBT_Tag *t)
{
	if(checkType(t))
		item_list.push_back(t);
}
