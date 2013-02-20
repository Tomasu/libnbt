#include "NBT_Tag_Byte_Array.h"
#include "NBT_File.h"

NBT_Tag_Byte_Array::~NBT_Tag_Byte_Array()
{
	if(data)
		free(data);

	data = 0;
	size = 0;
}

bool NBT_Tag_Byte_Array::read(NBT_File *fh)
{
	if(!fh->read(&size))
		return false;
	
	data = (uint8_t *)malloc(size);
	
	if(!fh->read(data, size))
	{
		free(data);
		return false;
	}
	
	return true;
}

bool NBT_Tag_Byte_Array::write(NBT_File *fh)
{
	if(!fh->write(size))
		return false;
	
	return fh->write(data, size);
}
