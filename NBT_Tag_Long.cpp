#include "NBT_Tag_Long.h"

#include "NBT_Buffer.h"

bool NBT_Tag_Long::decodeTag(NBT_Buffer *buff)
{
	if(!NBT_Tag::decodeTag(buff))
		return false;
	
	 if(!buff->readLong(&number))
		return false;
	
	return true;
}
