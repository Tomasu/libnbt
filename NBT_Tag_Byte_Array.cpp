#include "NBT_Tag_Byte_Array.h"
#include "NBT_File.h"

NBT_Tag_Byte_Array::~NBT_Tag_Byte_Array()
{
	if(data_)
		free(data_);

	data_ = 0;
	size = 0;
}

bool NBT_Tag_Byte_Array::read(NBT_File *fh)
{
	if(!fh->read(&size))
		return false;
	
	data_ = (uint8_t *)malloc(size);
	
	if(!fh->read(data_, size))
	{
		free(data_);
		data_ = 0;
		return false;
	}
	
	return true;
}

bool NBT_Tag_Byte_Array::write(NBT_File *fh)
{
	if(!fh->write(size))
		return false;
	
	return fh->write(data_, size);
}

void NBT_Tag_Byte_Array::setData(int32_t s, uint8_t *data)
{
	if(data_)
		free(data_);
	
	size = s;
	data_ = (uint8_t *)calloc(1, s);
	if(!data_)
		return;
	
	memcpy(data_, data, s);
}