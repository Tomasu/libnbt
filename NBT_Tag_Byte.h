#ifndef NBT_TAG_BYTE_H_GUARD
#define NBT_TAG_BYTE_H_GUARD

#include "NBT_Tag.h"

class NBT_Tag_Byte : public NBT_Tag
{
	public:
		NBT_Tag_Byte(bool named = true) : NBT_Tag(named, TAG_Byte), byte(0) { }
		~NBT_Tag_Byte() { }
		
		bool decodeTag(NBT_Buffer *buff);
		
		  int8_t value() { return byte; }

	private:
		int8_t byte;
};

#endif /* NBT_TAGBYTE_H_GUARD */
