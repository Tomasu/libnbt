#ifndef NBT_TAG_BYTE_H_GUARD
#define NBT_TAG_BYTE_H_GUARD

#include "NBT_Tag.h"

class NBT_Tag_Byte : public NBT_Tag
{
	public:
		NBT_Tag_Byte(bool named = true) : NBT_Tag(named, TAG_Byte), byte(0) { }
		~NBT_Tag_Byte() { }
		
		const char *className() { return "Byte"; }
		
		bool read(NBT_File *fh);
		bool write(NBT_File *fh);
		
		int8_t value() { return byte; }
		void setValue(int8_t b) { byte = b; }

		std::string serialize();
		
	private:
		int8_t byte;
};

#endif /* NBT_TAGBYTE_H_GUARD */
