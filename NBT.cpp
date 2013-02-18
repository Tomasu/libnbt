#include "NBT.h"
#include "NBT_Buffer.h"
#include "NBT_Debug.h"

NBT::NBT()
{
	
}

NBT::~NBT()
{
	
}

bool NBT::decode(NBT_File *fh)
{
	NBT_Debug("begin");
	
	bool ret = true;

	ret = decodeTag(fh);

	NBT_Debug("end");
	
	return ret;
}

bool NBT::encode(NBT_File *fh)
{
	NBT_Debug("begin");

	bool ret = encode(fh);
	
	NBT_Debug("end");

	return ret;
}
