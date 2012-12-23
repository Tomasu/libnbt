#ifndef NBT_TAG_INT_ARRAY_H_GUARD
#define NBT_TAG_INT_ARRAY_H_GUARD

#include "NBT_Tag.h"

class NBT_Tag_Int_Array : public NBT_Tag
{
	public:
		NBT_Tag_Int_Array(bool named = true) : NBT_Tag(named, TAG_Int_Array) { }
		~NBT_Tag_Int_Array();
		
		bool decodeTag(NBT_Buffer *buff);
		
	private:
		int32_t size;
		int32_t *data;
	
};

#endif /* NBT_TAG_INT_ARRAY_H_GUARD */
