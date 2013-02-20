#include "NBT_Tag_Long.h"

#include "NBT_File.h"

bool NBT_Tag_Long::read(NBT_File *fh)
{
	return fh->read(&number);
}

bool NBT_Tag_Long::write(NBT_File *fh)
{
	return fh->write(number);
}
