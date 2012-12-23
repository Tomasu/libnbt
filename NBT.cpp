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
	
	NBT_Buffer *buff = new NBT_Buffer(data, len);
	if(!decodeTag(buff))
		return false;
	
	delete buff;

	NBT_Debug("end");
	
	return true;
}

bool NBT::encode(void *&data, int &len)
{
	NBT_Debug("begin");



	NBT_Debug("end");

	return true;
}
