#ifndef NBT_TAG_SHORT_H_GUARD
#define NBT_TAG_SHORT_H_GUARD

#include "NBT_Tag.h"

class NBT_Buffer;

class NBT_Tag_Short : public NBT_Tag
{
	public:
      const char *className() { return "Short"; }
		NBT_Tag_Short(bool named = true) : NBT_Tag(named, TAG_Short), number(0) {}
		~NBT_Tag_Short() {}
		
		bool read(NBT_File *fh);
		bool write(NBT_File *fh);
		
		int16_t value() { return number; }
		void setValue(int16_t value) { number = value; }

		std::string serialize();
		
	private:
		int16_t number;
};

#endif /* NBT_TAG_SHORT_H_GUARD */
