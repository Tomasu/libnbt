#include "NBT_Tag_Int_Array.h"
#include "NBT_File.h"
#include "NBT_Debug.h"

NBT_Tag_Int_Array::~NBT_Tag_Int_Array()
{
	 if(data)
		 free(data);

	 data = 0;
	 size = 0;
}

bool NBT_Tag_Int_Array::read(NBT_File *fh)
{
	if(!fh->read(&size))
		return false;
	
	data = (int32_t *)malloc(size * sizeof(uint32_t));
	
	for(int i = 0; i < size; i++)
	{
		if(!fh->read(&data[i]))
		{
			free(data);
			return false;
		}
	}
	
	return true;
}

bool NBT_Tag_Int_Array::write(NBT_File *fh)
{
	if(!fh->write(size))
	{
		NBT_Error("failed to write array size");
		return false;
	}
	
	for(int i = 0; i < size; i++)
	{
		if(!fh->write(data[i]))
		{
			NBT_Error("failed to write array element [%i]", i);
			return false;
		}
	}
	
	return true;
}
