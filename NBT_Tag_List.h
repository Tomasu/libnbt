#ifndef NBT_TAG_LIST_H_GUARD
#define NBT_TAG_LIST_H_GUARD

#include <vector>
#include "NBT_Tag.h"
#include "NBT_Debug.h"

class NBT_Tag_List : public NBT_Tag
{
	public:
      const char *className() { return "List"; }
		NBT_Tag_List(bool named = true);
		~NBT_Tag_List();
		
		std::string serialize();
		
		bool read(NBT_File *fh);
		bool write(NBT_File *fh);
		
		const std::vector<NBT_Tag *> &items() { return item_list; }

		NBT_Tag *itemAt(uint32_t i) { return item_list[i]; }

		void setItemAt(uint32_t i, NBT_Tag *t);
		void insertItem(uint32_t i, NBT_Tag *t);
		void addItem(NBT_Tag *t);
		
		int count() { return item_list.size(); }

	private:
		uint8_t itemType;
		std::vector<NBT_Tag *> item_list;
	
		inline bool checkType(NBT_Tag *t)
		{
			if(t->type() != itemType)
			{
				NBT_Error("attempted to insert %s into list containing %s", tagNames[t->type()], tagNames[itemType]);
				return false;
			}
			
			return true;
		}
};

#endif /* NBT_TAG_LIST_H_GUARD */
