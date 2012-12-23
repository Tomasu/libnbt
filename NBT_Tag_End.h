#ifndef NBT_TAG_END_H_GUARD
#define NBT_TAG_END_H_GUARD

#include "NBT_Tag.h"

class NBT_Tag_End : public NBT_Tag
{
	public:
		NBT_Tag_End(bool named = false) : NBT_Tag(named, TAG_End) { }
		~NBT_Tag_End() { }
};

#endif /* NBT_TAG_END_H_GUARD */
