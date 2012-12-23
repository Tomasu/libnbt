#ifndef NBT_TAG_FLOAT_H_GUARD
#define NBT_TAG_FLOAT_H_GUARD

#include "NBT_Tag.h"

class NBT_Tag_Float : public NBT_Tag
{
	public:
		  NBT_Tag_Float(bool named = true) : NBT_Tag(named, TAG_Float), number(0.0) {}
		~NBT_Tag_Float() {}
		
		bool decodeTag(NBT_Buffer *buff);
		
		  float value() { return number; }

	private:
		  float number;
};

#endif /* NBT_TAG_FLOAT_H_GUARD */
