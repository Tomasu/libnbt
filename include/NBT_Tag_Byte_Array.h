#ifndef NBT_TAG_BYTE_ARRAY_H_GUARD
#define NBT_TAG_BYTE_ARRAY_H_GUARD

#include "NBT_Tag.h"

class NBT_Tag_Byte_Array : public NBT_Tag
{
	public:
		  NBT_Tag_Byte_Array(bool named = true) : NBT_Tag(named, TAG_Byte_Array), size(0), data_(0) { }
		  ~NBT_Tag_Byte_Array();
		
		const char *className() { return "Byte_Array"; }
		
		bool read(NBT_File *fh);
		bool write(NBT_File *fh);
		
		void setData(int32_t size, uint8_t *data);
		
		uint8_t *data() { return data_; }
		int32_t getSize() { return size; }
	private:
		int32_t size;
		uint8_t *data_;
	
};

#endif /* NBT_TAG_BYTE_ARRAY_H_GUARD */
