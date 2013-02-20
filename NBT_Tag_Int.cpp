#include "NBT_Tag_Int.h"

#include "NBT_File.h"

bool NBT_Tag_Int::read(NBT_File *fh)
{
	return fh->read(&number);
}

bool NBT_Tag_Int::write(NBT_File *fh)
{
	return fh->write(number);
}