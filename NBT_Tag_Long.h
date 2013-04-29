#ifndef NBT_TAG_LONG_H_GUARD
#define NBT_TAG_LONG_H_GUARD

#include "NBT_Tag.h"

class NBT_Tag_Long : public NBT_Tag
{
	public:
		const char *className() { return "Long"; }
		NBT_Tag_Long(bool named = true) : NBT_Tag(named, TAG_Long), number(0) {}
		~NBT_Tag_Long() {}
		
		bool read(NBT_File *fh);
		bool write(NBT_File *fh);
		
		int64_t value() { return number; }
		void setValue(int64_t value) { number = value; }
	private:
		int64_t number;
};

#endif /* NBT_TAG_LONG_H_GUARD */
