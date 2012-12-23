#include "NBT_Tag.h"
#include "NBT_Buffer.h"

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
	
}

const char *NBT_Tag::className()
{
	return typeid(this).name();
}

std::string NBT_Tag::serialize()
{
	std::stringstream strstr;
	if(named())
		strstr << tagName;
	else
		strstr << this->className();

	strstr << "(" << this << ")";
	return strstr.str();
}

bool NBT_Tag::decodeTag(NBT_Buffer *buff)
{
	if(tagNamed && tagType != TAG_End)
	{
		uint8_t readType = 0;
		if(!buff->read(&readType))
			return false;
		
		if(readType != tagType)
		{
			NBT_Debug("read the wrong type from data >:(");
			return false;
		}
		
		if(!buff->read(&tagName))
			return false;
		
		NBT_Debug("%s(%i) %s", (tagType < TAG_LAST_ITEM) ? tagNames[tagType] : "UNKNOWN", tagType, tagName.c_str());
	}
	else
	{
		//NBT_Debug("%s(%i) %s ALT", (tagType < TAG_LAST_ITEM) ? tagNames[tagType] : "UNKNOWN", tagType, tagName.c_str());
	}
	
	return true;
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
			NBT_Error("unkown sub tag type (%i) >:(", id);
			return 0;
	}
	
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
