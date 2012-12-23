#include "NBT_Tag_Double.h"

#include "NBT_Buffer.h"

bool NBT_Tag_Double::decodeTag(NBT_Buffer *buff)
{
	if(!NBT_Tag::decodeTag(buff))
		return false;
	
	 if(!buff->readDouble(&number))
		return false;
	
	return true;
}
