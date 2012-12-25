#ifndef NBT_TAG_H_GUARD
#define NBT_TAG_H_GUARD

#include <string>

enum NBT_Tag_Type
{
	TAG_End = 0,
	TAG_Byte,
	TAG_Short,
	TAG_Int,
	TAG_Long,
	TAG_Float,
	TAG_Double,
	TAG_Byte_Array,
	TAG_String,
	TAG_List,
	TAG_Compound,
	TAG_Int_Array,
	TAG_LAST_ITEM
};

class NBT_Buffer;

class NBT_Tag
{
	public:
		virtual const char *className();

		NBT_Tag(bool named = true, int t = -1);
		virtual ~NBT_Tag();
		
		int type() { return tagType; }
		bool named() { return tagNamed; }
		const std::string &name() { return tagName; }

		virtual std::string serialize();
		
		virtual bool decodeTag(NBT_Buffer *buff);
		
		bool encode(NBT_Buffer *buff);
		virtual bool encodeTag(NBT_Buffer *buff) = 0;
		
		static NBT_Tag *tagFromType(uint8_t id, bool named = true);

		static const char *tagNames[TAG_LAST_ITEM];
	private:
		uint8_t tagType;
		bool tagNamed;
		std::string tagName;
	
};

#endif /* NBT_TAG_H_GUARD */
