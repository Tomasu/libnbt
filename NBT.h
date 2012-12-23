#ifndef NBT_H_GUARD
#define NBT_H_GUARD

#include <unordered_map>

#include "NBT_Tag_Compound.h"

class NBT : public NBT_Tag_Compound
{
	public:
		NBT();
		~NBT();
		
		bool decode(void *data, int len);
		bool encode(void *&data, int &len);

	private:

	
};

#endif /* NBT_H_GUARD */
