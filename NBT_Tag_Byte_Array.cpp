#include "NBT_Tag_Byte_Array.h"
#include "NBT_Buffer.h"

NBT_Tag_Byte_Array::~NBT_Tag_Byte_Array()
{
	if(data)
		free(data);

	data = 0;
	size = 0;
}

bool NBT_Tag_Byte_Array::decodeTag(NBT_Buffer *buff)
{
	if(!NBT_Tag::decodeTag(buff))
		return false;
	
	if(!buff->readInt(&size))
		return false;
	
	data = (uint8_t *)malloc(size);
	
	if(!buff->read(data, size))
	{
		free(data);
		return false;
	}
	
	return true;
}
