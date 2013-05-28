#include "NBT.h"
#include "NBT_Tag.h"
#include "NBT_Debug.h"
#include "NBT_File.h"

NBT::NBT() : NBT_Tag_Compound(true)
{
	
}

NBT::~NBT()
{
	
}

bool NBT::load(const std::string &path, bool compressed)
{
	//NBT_Debug("attempting to load file: %s", path.c_str());
	NBT_File *fh = new NBT_File(path);
	if(!fh->open())
	{
		//NBT_Error("failed to open nbt file :(");
		return false;
	}

	if(!fh->seek(0, SEEK_END))
	{
		//NBT_Error("failed to seek to end of file");
		return false;
	}

	uint32_t length = fh->tell();

	if(!fh->seek(0, SEEK_SET))
	{
		//NBT_Error("failed to seek to beginning of file");
		return false;
	}

	if(compressed)
	{
		if(!fh->readCompressedMode(length, false))
		{
			NBT_Error("failed to enter compressed mode for %i bytes :(", length);
			return false;
		}
	}
	
	if(!readTag(fh))
	{
		//NBT_Error("failed to read nbt data");
		delete fh;
		return false;
	}

	if(compressed)
		fh->endCompressedMode();

	int child_count = count();
	if(!child_count)
	{
		return false;
	}
	
	// remove pointless root tag if it exists
	//NBT_Debug("child_count: %i, tagName: %s", child_count, tagName.c_str());
	if(child_count == 2 && tagName == "")
	{
		//NBT_Debug("remove pointless root tag");
		NBT_Tag_Compound *child = (NBT_Tag_Compound *)children[0].second;
		if(child->type() == TAG_Compound)
		{
			NBT_TagMap &ch = child->_get_children();
			children.assign(ch.begin(), ch.end());
			tagName = child->name();
			tagNamed = child->named();
			tagRow = 0;
			tagParent = 0;
		}
	}
	
	return true;
}
