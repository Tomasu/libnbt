#ifndef NBT_TAG_INT_ARRAY_H_GUARD
#define NBT_TAG_INT_ARRAY_H_GUARD

#include "NBT_Tag.h"

class NBT_Tag_Int_Array : public NBT_Tag
{
	public:
      const char *className() { return "Int_Array"; }
		NBT_Tag_Int_Array(bool named = true) : NBT_Tag(named, TAG_Int_Array) { }
		~NBT_Tag_Int_Array();
		
		bool read(NBT_File *fh);
		bool write(NBT_File *fh);
		
		int32_t *data() { return data_; }

		void setData(int32_t size, int32_t *data);
	private:
		int32_t size;
		int32_t *data_;
	
};

#endif /* NBT_TAG_INT_ARRAY_H_GUARD */
