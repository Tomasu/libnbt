#include "NBT_Tag.h"
#include "NBT_File.h"

#include "NBT_Tag_End.h"
#include "NBT_Tag_Byte.h"
#include "NBT_Tag_Short.h"
#include "NBT_Tag_Int.h"
#include "NBT_Tag_Long.h"
#include "NBT_Tag_Float.h"
#include "NBT_Tag_Double.h"
#include "NBT_Tag_Byte_Array.h"
#include "NBT_Tag_String.h"
#include "NBT_Tag_List.h"
#include "NBT_Tag_Compound.h"
#include "NBT_Tag_Int_Array.h"

#include "NBT_Debug.h"

#include <sstream>
#include <typeinfo>

NBT_Tag::NBT_Tag(bool named, int t) : tagType(t), tagNamed(named) 
{
	
}

NBT_Tag::~NBT_Tag()
{
	//NBT_Debug("delete %s \"%s\"", tagNames[tagType], tagName.c_str());
}

const char *NBT_Tag::className()
{
	return typeid(this).name()+2;
}

std::string NBT_Tag::serialize()
{
	std::stringstream strstr;
   strstr << this->className();

   if(named())
      strstr << "(" << this << "; " << name() << ")";
   else
      strstr << "(" << this << ")";
      
	return strstr.str();
}

bool NBT_Tag::writeTag(NBT_File *fh)
{	
	if(!fh->write(tagType))
	{
		NBT_Error("failed to write tag type");
		return false;
	}
	
	if(tagNamed)
	{
		if(!fh->write(tagName))
		{
			NBT_Error("failed to write tag name");
			return false;
		}
	}
	
	return write(fh);
}

NBT_Tag *NBT_Tag::readTag(NBT_File *fh, bool named)
{
	//NBT_Debug("begin");
	
	NBT_Tag *tag = 0;
	
	uint8_t tagType = TAG_UNKNOWN;
	
	if(!fh->read(&tagType))
	{
		NBT_Error("failed to read tag type :(");
		return 0;
	}

	tag = tagFromType(tagType, named);
	if(!tag)
	{
		NBT_Error("failed to create tag from type %i", tagType);
		return 0;
	}
	
	// don't read in name if we're an unnamed tag
	// TAG_End is special, it is never named
	if(named && tagType != TAG_End)
	{
		if(!fh->read(tag->tagName))
		{
			NBT_Error("failed to read tag name :(");
			delete tag;
			return 0;
		}
	}
	
	if(!tag->read(fh))
	{
		NBT_Error("failed to read tag specifics");
		delete tag;
		return 0;
	}
	
	//NBT_Debug("end");
	return tag;
}

NBT_Tag *NBT_Tag::tagFromType(uint8_t id, bool named)
{
	NBT_Tag *tag = 0;
	
	switch(id)
	{
		case TAG_End:
			tag = new NBT_Tag_End(named);
			break;
		
		case TAG_Byte:
			tag = new NBT_Tag_Byte(named);
			break;
			
		case TAG_Short:
			tag = new NBT_Tag_Short(named);
			break;
			
		case TAG_Int:
			tag = new NBT_Tag_Int(named);
			break;
			
		case TAG_Long:
			tag = new NBT_Tag_Long(named);
			break;
			
		case TAG_Float:
			tag = new NBT_Tag_Float(named);
			break;
			
		case TAG_Double:
			tag = new NBT_Tag_Double(named);
			break;
			
		case TAG_Byte_Array:
			tag = new NBT_Tag_Byte_Array(named);
			break;
			
		case TAG_String:
			tag = new NBT_Tag_String(named);
			break;
			
		case TAG_List:
			tag = new NBT_Tag_List(named);
			break;
			
		case TAG_Compound:
			tag = new NBT_Tag_Compound(named);
			break;
		
		case TAG_Int_Array:
			tag = new NBT_Tag_Int_Array(named);
			break;
			
		default:
			NBT_Error("unkown tag type (%i) >:(", id);
			return 0;
	}
	
	//NBT_Debug("new tag from type: %s", tagNames[id]);
	
	return tag;
}

const char *NBT_Tag::tagNames[TAG_LAST_ITEM]
{
	"TAG_End",
	"TAG_Byte",
	"TAG_Short",
	"TAG_Int",
	"TAG_Long",
	"TAG_Float",
	"TAG_Double",
	"TAG_Byte_Array",
	"TAG_String",
	"TAG_List",
	"TAG_Compound",
	"TAG_Int_Array"
};
