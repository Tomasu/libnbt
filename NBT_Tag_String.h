#ifndef NBT_TAG_STRING_H_GUARD
#define NBT_TAG_STRING_H_GUARD

#include <string>
#include "NBT_Tag.h"

class NBT_Buffer;

class NBT_Tag_String : public NBT_Tag
{
	public:
      const char *className() { return "String"; }
		NBT_Tag_String(bool named = true) : NBT_Tag(named, TAG_String) { }
		~NBT_Tag_String() { }
		
		bool read(NBT_File *fh);
		bool write(NBT_File *fh);
		
		const std::string &value() { return str; }

	private:
		std::string str;
};

#endif /* NBT_TAG_STRING_H_GUARD */
