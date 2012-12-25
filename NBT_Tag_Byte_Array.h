#ifndef NBT_TAG_BYTE_ARRAY_H_GUARD
#define NBT_TAG_BYTE_ARRAY_H_GUARD

#include "NBT_Tag.h"

class NBT_Tag_Byte_Array : public NBT_Tag
{
	public:
		  NBT_Tag_Byte_Array(bool named = true) : NBT_Tag(named, TAG_Byte_Array), size(0), data(0) { }
		  ~NBT_Tag_Byte_Array();
		
		const char *className() { return "Byte_Array"; }
		
		bool decodeTag(NBT_Buffer *buff);
		bool encodeTag(NBT_Buffer *buff);
		
	private:
		int32_t size;
		uint8_t *data;
	
};

#endif /* NBT_TAG_BYTE_ARRAY_H_GUARD */
