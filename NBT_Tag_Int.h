#ifndef NBT_TAG_INT_H_GUARD
#define NBT_TAG_INT_H_GUARD

#include "NBT_Tag.h"

class NBT_Tag_Int : public NBT_Tag
{
	public:
      const char *className() { return "Int"; }
		  NBT_Tag_Int(bool named = true) : NBT_Tag(named, TAG_Int), number(0.0) {}
		~NBT_Tag_Int() {}
		
		bool decodeTag(NBT_Buffer *buff);
		
		  int32_t value() { return number; }

	private:
		  int32_t number;
};

#endif /* NBT_TAG_INT_H_GUARD */
