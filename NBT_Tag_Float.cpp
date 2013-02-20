#include "NBT_Tag_Float.h"

#include "NBT_File.h"

bool NBT_Tag_Float::read(NBT_File *fh)
{
	return fh->read(&number);
}

bool NBT_Tag_Float::write(NBT_File *fh)
{
	return fh->write(number);
}
