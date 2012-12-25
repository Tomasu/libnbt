#include "NBT_Tag_Compound.h"

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
#include "NBT_Tag_Int_Array.h"

#include "NBT_Debug.h"

#include <algorithm>
#include <sstream>

NBT_Tag_Compound::NBT_Tag_Compound(bool named) : NBT_Tag(named, TAG_Compound)
{
	
}

NBT_Tag_Compound::~NBT_Tag_Compound()
{
	for(auto &child: children)
	 {
			delete child.second;
	 }
		
	children.clear();
}

std::string NBT_Tag_Compound::serialize()
{
	std::stringstream str;
	if(named())
      str << "Compound(" << name() << ")";
      
   if(count() < 1)
   {
      str << " { }";
      return str.str();
   }
   
   str << " {\n";
	for (auto &child: children)
	{
      if(child.second->type() == TAG_Compound && ((NBT_Tag_Compound*)child.second)->count() < 1)
         continue; //str << "; ";
      else
      if(child.second->type() == TAG_List && ((NBT_Tag_List*)child.second)->count() < 1)
         continue; //str << "; ";
      else
         str << child.second->serialize() << "; ";
	}

	str << "};\n";
	return str.str();
}

bool NBT_Tag_Compound::decodeTag(NBT_Buffer *buff)
{
	if(!NBT_Tag::decodeTag(buff))
		return false;
	
	NBT_Debug("Compound \"%s\" begin", name().c_str());
	
	NBT_Tag *tag = 0;
	
	do {
		uint8_t id = 0;
		if(!buff->peek(&id))
		{
			NBT_Error("failed to load tag in compound tag");
			break;
		}

		tag = tagFromType(id);
		if(!tag)
			break;
		
		if(id == TAG_End) // skip tag end
			break;

		if(!tag->decodeTag(buff))
		{
			NBT_Error("failed to load tag in compound tag");
			break;
		}

		children.insert(std::pair<std::string,NBT_Tag*>(tag->name(), tag));
		
	} while(tag->type() != TAG_End);
	
	if(!tag || tag->type() != TAG_End)
	{
		NBT_Error("malformed compound chunk, missing TAG_End");
		
		for(auto &child: children)
		{
			delete child.second;
		}
		
		children.clear();
		
		return false;
	}
	
	NBT_Debug("Compound \"%s\" end", name().c_str());
	return true;
}

bool NBT_Tag_Compound::encodeTag(NBT_Buffer *buff)
{
	for(auto &child: children)
	{
		if(!child.second->encode(buff))
		{
			NBT_Error("failed to encode tag, bailing");
			return false;
		}
	}
	
	return true;
}

const std::vector<std::string> NBT_Tag_Compound::keys()
{
	std::vector<std::string> keys;
	if(!children.size())
		return std::vector<std::string>();

	keys.reserve(children.size());
	for(auto &pair: children)
	{
		keys.push_back(pair.first);
	}

	return keys;
}

bool NBT_Tag_Compound::hasKey(const std::string &key)
{
	return children.count(key) > 0;
}

NBT_Tag *NBT_Tag_Compound::get(const std::string &key)
{
	if(children.count(key))
		return children.at(key);

	return 0;
}

int8_t NBT_Tag_Compound::getByte(const std::string &key)
{
	if(children.count(key))
		return dynamic_cast<NBT_Tag_Byte *>(children.at(key))->value();

	return 0;
}

int16_t NBT_Tag_Compound::getShort(const std::string &key)
{
	if(children.count(key))
		return dynamic_cast<NBT_Tag_Short *>(children.at(key))->value();

	return 0;
}

int32_t NBT_Tag_Compound::getInt(const std::string &key)
{
	if(children.count(key))
		return dynamic_cast<NBT_Tag_Int *>(children.at(key))->value();

	return 0;
}

int64_t NBT_Tag_Compound::getLong(const std::string &key)
{
	if(children.count(key))
		return dynamic_cast<NBT_Tag_Long *>(children.at(key))->value();

	return 0;
}

float NBT_Tag_Compound::getFloat(const std::string &key)
{
	if(children.count(key))
		return dynamic_cast<NBT_Tag_Float *>(children.at(key))->value();

	return 0;
}

double NBT_Tag_Compound::getDouble(const std::string &key)
{
	if(children.count(key))
		return dynamic_cast<NBT_Tag_Double *>(children.at(key))->value();

	return 0;
}

std::string NBT_Tag_Compound::getString(const std::string &key)
{
	if(children.count(key))
		return dynamic_cast<NBT_Tag_String *>(children.at(key))->value();

	return std::string();
}


NBT_Tag_Compound *NBT_Tag_Compound::getCompound(const std::string &key)
{
	if(children.count(key))
		return dynamic_cast<NBT_Tag_Compound *>(children.at(key));

	return 0;
}

NBT_Tag_List *NBT_Tag_Compound::getList(const std::string &key)
{
	if(children.count(key))
		return dynamic_cast<NBT_Tag_List *>(children.at(key));

	return 0;
}

NBT_Tag_Byte_Array *NBT_Tag_Compound::getByteArray(const std::string &key)
{
	 return dynamic_cast<NBT_Tag_Byte_Array *>(children.at(key));
}

NBT_Tag_Int_Array *NBT_Tag_Compound::getIntArray(const std::string &key)
{
	 return dynamic_cast<NBT_Tag_Int_Array *>(children.at(key));
}

