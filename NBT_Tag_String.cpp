#include "NBT_Tag_String.h"
#include "NBT_Buffer.h"

bool NBT_Tag_String::decodeTag(NBT_Buffer *buff)
{
	if(!NBT_Tag::decodeTag(buff))
		return false;
	
	 if(!buff->read(&str))
		return false;
	
	return true;
}

bool NBT_Tag_String::encodeTag(NBT_Buffer *buff)
{
	return buff->write(str);
}
