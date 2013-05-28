#ifndef NBT_H_GUARD
#define NBT_H_GUARD

#include <unordered_map>

#include "NBT_Tag_Compound.h"

class NBT_File;

class NBT : public NBT_Tag_Compound
{
	public:
		NBT();
		~NBT();
		
		bool load(const std::string &path, bool compressed = true);
};

#endif /* NBT_H_GUARD */
