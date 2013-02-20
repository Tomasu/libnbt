#include "NBT_Tag_Short.h"

#include "NBT_File.h"

bool NBT_Tag_Short::read(NBT_File *fh)
{
	return fh->read(&number);
}

bool NBT_Tag_Short::write(NBT_File *fh)
{
	return fh->write(number);
}
