#include "NBT.h"
#include "NBT_Debug.h"

NBT::NBT()
{
	
}

NBT::~NBT()
{
	
}

bool NBT::read(NBT_File *fh)
{
	NBT_Debug("begin");
	
	bool ret = true;

	ret = readTag(fh);

	NBT_Debug("end");
	
	return ret;
}

bool NBT::write(NBT_File *fh)
{
	//NBT_Debug("begin");

	bool ret = writeTag(fh);
	
	//NBT_Debug("end");

	return ret;
}
