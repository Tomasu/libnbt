#include "NBT_Tag_Byte.h"
#include "NBT_File.h"
#include <sstream>

bool NBT_Tag_Byte::read(NBT_File *fh)
{	
	return fh->read(&byte);
}

bool NBT_Tag_Byte::write(NBT_File *fh)
{
	return fh->write(byte);
}

std::string NBT_Tag_Byte::serialize()
{
	std::stringstream strstr;
   strstr << this->className();

   if(named())
      strstr << "(" << this << "; " << name() << "; " <<  (int)byte << ")";
   else
      strstr << "(" << this << "; " << (int)byte << ")";
      
	return strstr.str();
}