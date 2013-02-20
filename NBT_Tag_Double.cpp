#include "NBT_Tag_Double.h"

#include "NBT_File.h"

bool NBT_Tag_Double::read(NBT_File *fh)
{
	return fh->read(&number);
}

bool NBT_Tag_Double::write(NBT_File *fh)
{
	return fh->write(number);
}
