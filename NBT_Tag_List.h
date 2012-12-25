#ifndef NBT_TAG_LIST_H_GUARD
#define NBT_TAG_LIST_H_GUARD

#include <vector>
#include "NBT_Tag.h"

class NBT_Tag_List : public NBT_Tag
{
	public:
      const char *className() { return "List"; }
		NBT_Tag_List(bool named = true);
		~NBT_Tag_List();
		
		std::string serialize();
		
		bool decodeTag(NBT_Buffer *buff);
		bool encodeTag(NBT_Buffer *buff);
		
		const std::vector<NBT_Tag *> &items() { return item_list; }

		int count() { return item_list.size(); }

	private:
		uint8_t itemType;
		std::vector<NBT_Tag *> item_list;
	
};

#endif /* NBT_TAG_LIST_H_GUARD */
