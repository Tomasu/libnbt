#include "NBT_Tag_Compound.h"

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
      str << "Compound(" << tagName << ")";
      
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

bool NBT_Tag_Compound::read(NBT_File *fh)
{
	//NBT_Debug("Compound \"%s\" begin", name().c_str());
	
	NBT_Tag *tag = 0;
	int idx = 0;

	do {
		tag = LoadTag(fh);
		if(!tag)
		{
			NBT_Error("failed to load child tag.");
			break;
		}
		
		tag->setParent(this);
		tag->setRow(idx++);

		//NBT_Debug("read tag: %s %s", tagNames[tag->type()], tag->name().c_str());
		//if(tag->type() != TAG_End)

		children.push_back(std::pair<std::string,NBT_Tag*>(tag->name(), tag));
		
	} while(tag->type() != TAG_End);
	
	if(!tag || tag->type() != TAG_End)
	{
		NBT_Error("malformed compound tag, missing TAG_End");
		
		for(auto &child: children)
		{
			delete child.second;
		}
		
		children.clear();
		
		return false;
	}
	
	//NBT_Debug("Compound \"%s\" end %i", name().c_str(), children.size());
	return true;
}

bool NBT_Tag_Compound::write(NBT_File *fh)
{
	//NBT_Debug("write %i children. named:%i", children.size(), named());
	for(auto &child: children)
	{
	//	NBT_Debug("write: named:%i %s::%s", child.second->named(), named() ? name().c_str() : tagNames[type()], child.second->named() ? child.second->name().c_str() : tagNames[child.second->type()]);
		if(!child.second->writeTag(fh))
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
	return get(key) != 0;
}

NBT_Tag *NBT_Tag_Compound::get(const std::string &key)
{
	for(std::pair<std::string, NBT_Tag *> &pair: children)
	{
		if(pair.first == key)
		{
			return pair.second;
		}
	}

	return 0;
}

void NBT_Tag_Compound::set(const std::string &key, NBT_Tag *v)
{
	for(std::pair<std::string, NBT_Tag *> &pair: children)
	{
		if(pair.first == key)
		{
			pair.second = v;
			return;
		}
	}
	
	NBT_TagMap::iterator it = children.end();
	it--;
	if(it->second->type() == TAG_End)
		children.insert(it, std::pair<std::string, NBT_Tag *>(key, v));
	else
		children.push_back(std::pair<std::string, NBT_Tag *>(key, v));
}

int8_t NBT_Tag_Compound::getByte(const std::string &key)
{
	NBT_Tag *tag = get(key);
	if(tag)
		return dynamic_cast<NBT_Tag_Byte *>(tag)->value();

	return 0;
}

int16_t NBT_Tag_Compound::getShort(const std::string &key)
{
	NBT_Tag *tag = get(key);
	if(tag)
		return dynamic_cast<NBT_Tag_Short *>(tag)->value();

	return 0;
}

int32_t NBT_Tag_Compound::getInt(const std::string &key)
{
	NBT_Tag *tag = get(key);
	if(tag)
		return dynamic_cast<NBT_Tag_Int *>(tag)->value();

	return 0;
}

int64_t NBT_Tag_Compound::getLong(const std::string &key)
{
	NBT_Tag *tag = get(key);
	if(tag)
		return dynamic_cast<NBT_Tag_Long *>(tag)->value();

	return 0;
}

float NBT_Tag_Compound::getFloat(const std::string &key)
{
	NBT_Tag *tag = get(key);
	if(tag)
		return dynamic_cast<NBT_Tag_Float *>(tag)->value();

	return 0;
}

double NBT_Tag_Compound::getDouble(const std::string &key)
{
	NBT_Tag *tag = get(key);
	if(tag)
		return dynamic_cast<NBT_Tag_Double *>(tag)->value();

	return 0;
}

std::string NBT_Tag_Compound::getString(const std::string &key)
{
	NBT_Tag *tag = get(key);
	if(tag)
		return dynamic_cast<NBT_Tag_String *>(tag)->value();

	return std::string();
}


NBT_Tag_Compound *NBT_Tag_Compound::getCompound(const std::string &key)
{
	NBT_Tag *tag = get(key);
	if(tag)
		return dynamic_cast<NBT_Tag_Compound *>(tag);

	return 0;
}

NBT_Tag_List *NBT_Tag_Compound::getList(const std::string &key)
{
	NBT_Tag *tag = get(key);
	if(tag)
		return dynamic_cast<NBT_Tag_List *>(tag);

	return 0;
}

NBT_Tag_Byte_Array *NBT_Tag_Compound::getByteArray(const std::string &key)
{
	NBT_Tag *tag = get(key);
	if(tag)
		return dynamic_cast<NBT_Tag_Byte_Array *>(tag);

	return 0;
}

NBT_Tag_Int_Array *NBT_Tag_Compound::getIntArray(const std::string &key)
{
	NBT_Tag *tag = get(key);
	if(tag)
		return dynamic_cast<NBT_Tag_Int_Array *>(tag);

	return 0;
}

NBT_Tag *NBT_Tag_Compound::childAt(int idx)
{
	return children.at(idx).second;
}

NBT_Tag *NBT_Tag_Compound::remove(NBT_Tag *tag)
{
	uint32_t idx = tag->row();
	
	if(idx >= children.size())
		return 0;
	
	children.erase(children.begin()+idx);
	
	// update row index
	for(uint32_t i = idx; i < children.size(); i++)
	{
		children[i].second->setRow(i);
	}
	
	return tag;
}
