#ifndef NBT_TAG_H_GUARD
#define NBT_TAG_H_GUARD

#include <string>

enum NBT_Tag_Type
{
	TAG_UNKNOWN = -1,
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

class NBT_File;

class NBT_Tag
{
	public:
		virtual const char *className();

		NBT_Tag(NBT_Tag *parent, bool named = true, int t = -1);
		NBT_Tag(bool named = true, int t = -1);
		virtual ~NBT_Tag();
		
		int type() { return tagType; }
		bool named() { return tagNamed; }
		const std::string &name() { return tagName; }

		NBT_Tag *parent() { return tagParent; }
		void setParent(NBT_Tag *parent) { tagParent = parent; }

		virtual std::string serialize();

		// create a new tag from the given tag type
		// doesn't do any file reading
		static NBT_Tag *tagFromType(uint8_t id, bool named = true);

		// read tag from NBT_File, named or unnamed
		static NBT_Tag *LoadTag(NBT_File *fh, bool named = true);
		bool readTag(NBT_File *fh, bool named = true);
		
		// read tag specific data from NBT_File
		virtual bool read(NBT_File *fh) = 0;
		
		// write tag to NBT_File
		bool writeTag(NBT_File *fh);
		
		// write tag specific data to NBT_File
		virtual bool write(NBT_File *fh) = 0;

		int row() { return tagRow; }
		void setRow(int row) { tagRow = row; }

		static const char *tagNames[TAG_LAST_ITEM];
	protected:
		NBT_Tag *tagParent;
		uint8_t tagType;
		bool tagNamed;
		std::string tagName;
		int tagRow;
};

#endif /* NBT_TAG_H_GUARD */
