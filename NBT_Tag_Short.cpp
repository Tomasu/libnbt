#include "NBT_Tag_Short.h"

#include "NBT_Buffer.h"

bool NBT_Tag_Short::decodeTag(NBT_Buffer *buff)
{
	if(!NBT_Tag::decodeTag(buff))
		return false;
	
	 if(!buff->readShort(&number))
		return false;
	
	return true;
}

bool NBT_Tag_Short::encodeTag(NBT_Buffer *buff)
{
	return buff->writeShort(number);
}
