#ifndef NBT_TAG_COMPOUND_H_GUARD
#define NBT_TAG_COMPOUND_H_GUARD

#include <map>
#include <vector>

#include "NBT_Tag.h"

typedef std::vector< std::pair<std::string, NBT_Tag *> > NBT_TagMap;

class NBT_Tag_List;
class NBT_Tag_Byte_Array;
class NBT_Tag_Int_Array;
class NBT_Tag_Byte;
class NBT_Tag_Double;
class NBT_Tag_Float;
class NBT_Tag_Int;
class NBT_Tag_Long;
class NBT_Tag_Short;
class NBT_Tag_String;

template<typename T>
struct MapTagType;

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
		
		/*
		template<typename T>
			typename MapTagType<T>::type getValue(T *tag, typename MapTagType<T>::type def = MapTagType<T>::def)
		{
			T *tag = dynamic_cast<T>(tag);
			if(!tag)
				return def;
			
			return tag->value();
		}*/
};

/*
template<>
struct MapTagType<NBT_Tag_Byte>
{
	typedef int8_t type;
	const static type def = 0;
}

template<>
struct MapTagType<NBT_Tag_Double>
{
	typedef double type;
	const static type def = 0.0;
}

template<>
struct MapTagType<NBT_Tag_Float>
{
	typedef float type;
	const static type def = 0.0f;
}

template<>
struct MapTagType<NBT_Tag_Int>
{
	typedef int32_t type;
	const static type def = 0;
}

template<>
struct MapTagType<NBT_Tag_Long>
{
	typedef int64_t type;
	const static type def = 0;
}

template<>
struct MapTagType<NBT_Tag_Short>
{
	typedef int16_t type;
	const static type def = 0;
}

template<>
struct MapTagType<NBT_Tag_String>
{
	typedef std::string type;
	const static type def = std::string();
}
*/

#endif /* NBT_TAG_COMPOUND_H_GUARD */
