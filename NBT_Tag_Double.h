#ifndef NBT_TAG_DOUBLE_H_GUARD
#define NBT_TAG_DOUBLE_H_GUARD

#include "NBT_Tag.h"

class NBT_Tag_Double : public NBT_Tag
{
	public:
      const char *className() { return "Double"; }
      
		  NBT_Tag_Double(bool named = true) : NBT_Tag(named, TAG_Double), number(0.0) {}
		~NBT_Tag_Double() {}
		
		bool decodeTag(NBT_Buffer *buff);
		
		  double value() { return number; }

	private:
		  double number;
};

#endif /* NBT_TAG_DOUBLE_H_GUARD */
