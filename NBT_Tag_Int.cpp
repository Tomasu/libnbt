#include "NBT_Tag_Int.h"

#include "NBT_Buffer.h"

bool NBT_Tag_Int::decodeTag(NBT_Buffer *buff)
{
	if(!NBT_Tag::decodeTag(buff))
		return false;
	
	 if(!buff->readInt(&number))
		return false;
	
	return true;
}

bool NBT_Tag_Int::encodeTag(NBT_Buffer *buff)
{
	return buff->writeInt(number);
}