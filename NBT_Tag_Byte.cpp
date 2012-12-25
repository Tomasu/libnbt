#include "NBT_Tag_Byte.h"
#include "NBT_Buffer.h"

bool NBT_Tag_Byte::decodeTag(NBT_Buffer *buff)
{
	if(!NBT_Tag::decodeTag(buff))
		return false;
	
	if(!buff->read(&byte))
		return false;
	
	return true;
}

bool NBT_Tag_Byte::encodeTag(NBT_Buffer *buff)
{
	return buff->write(byte);
}
