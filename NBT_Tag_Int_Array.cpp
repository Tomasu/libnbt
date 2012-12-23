#include "NBT_Tag_Int_Array.h"
#include "NBT_Buffer.h"

NBT_Tag_Int_Array::~NBT_Tag_Int_Array()
{
	 if(data)
		 free(data);

	 data = 0;
	 size = 0;
}

bool NBT_Tag_Int_Array::decodeTag(NBT_Buffer *buff)
{
	if(!NBT_Tag::decodeTag(buff))
		return false;
	
	if(!buff->readInt(&size))
		return false;
	
	data = (int32_t *)malloc(size * sizeof(uint32_t));
	
	for(int i = 0; i < size; i++)
	{
		if(!buff->readInt(&data[i]))
		{
			free(data);
			return false;
		}
	}
	
	return true;
}
