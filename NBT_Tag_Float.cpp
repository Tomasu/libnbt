#include "NBT_Tag_Float.h"

#include "NBT_Buffer.h"

bool NBT_Tag_Float::decodeTag(NBT_Buffer *buff)
{
	if(!NBT_Tag::decodeTag(buff))
		return false;
	
	 if(!buff->readFloat(&number))
		return false;
	
	return true;
}
