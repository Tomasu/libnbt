#include "NBT.h"
#include "NBT_Buffer.h"
#include "NBT_Debug.h"

NBT::NBT()
{
	
}

NBT::~NBT()
{
	
}

bool NBT::decode(void *data, int len)
{
	NBT_Debug("begin");
	
	bool ret = true;
	
	NBT_Buffer *buff = new NBT_Buffer(data, len);
	ret = decodeTag(buff);
	
	delete buff;

	NBT_Debug("end");
	
	return ret;
}

bool NBT::encode(NBT_Buffer *buff)
{
	NBT_Debug("begin");

	bool ret = encode(buff);
	
	NBT_Debug("end");

	return ret;
}
