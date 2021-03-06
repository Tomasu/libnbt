#include "NBT_Tag_String.h"
#include "NBT_File.h"

bool NBT_Tag_String::read(NBT_File *fh)
{
	return fh->read(str);
}

bool NBT_Tag_String::write(NBT_File *fh)
{
	return fh->write(str);
}

std::string NBT_Tag_String::serialize()
{
	std::stringstream strstr;
   strstr << this->className();

   if(named())
      strstr << "(" << this << "; " << name() << "; " <<  str << ")";
   else
      strstr << "(" << this << "; " << str << ")";
      
	return strstr.str();
}