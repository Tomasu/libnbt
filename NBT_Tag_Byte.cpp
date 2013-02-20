#include "NBT_Tag_Byte.h"
#include "NBT_File.h"

bool NBT_Tag_Byte::read(NBT_File *fh)
{	
	return fh->read(&byte);
}

bool NBT_Tag_Byte::write(NBT_File *fh)
{
	return fh->write(byte);
}
