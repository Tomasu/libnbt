#ifndef NBT_TAG_COMPOUND_H_GUARD
#define NBT_TAG_COMPOUND_H_GUARD

#include <map>
#include <vector>

#include "NBT_Tag.h"

typedef std::vector< std::pair<std::string, NBT_Tag *> > NBT_TagMap;

class NBT_Tag_List;
class NBT_Tag_Byte_Array;
class NBT_Tag_Int_Array;

class NBT_Tag_Compound : public NBT_Tag
{
	public:
		NBT_Tag_Compound(bool named = true);
		~NBT_Tag_Compound();
		std::string serialize();

		bool read(NBT_File *fh);
		bool write(NBT_File *fh);
		
		bool hasKey(const std::string &key);
		
		NBT_Tag *get(const std::string &key);
		void set(const std::string &key, NBT_Tag *v);
		
		int8_t getByte(const std::string &key);
		int16_t getShort(const std::string &key);
		int32_t getInt(const std::string &key);
		int64_t getLong(const std::string &key);
		float getFloat(const std::string &key);
		double getDouble(const std::string &key);
		std::string getString(const std::string &key);
		
		NBT_Tag_Compound *getCompound(const std::string &key);
		NBT_Tag_List *getList(const std::string &key);
		NBT_Tag_Byte_Array *getByteArray(const std::string &key);
		NBT_Tag_Int_Array *getIntArray(const std::string &key);
		
		//void set(const std::string &key, NBT_Tag *tag);
		
		const std::vector<std::string> keys();

		int count() { return children.size(); }
		
		NBT_Tag *childAt(int idx);

		NBT_Tag *remove(NBT_Tag *);
		
		// not really public...
		NBT_TagMap &_get_children() { return children; }
		
	protected:
		NBT_TagMap children;
};

#endif /* NBT_TAG_COMPOUND_H_GUARD */