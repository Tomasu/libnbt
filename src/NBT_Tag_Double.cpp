#include "NBT_Tag_Double.h"

#include "NBT_File.h"

#include <sstream>
bool NBT_Tag_Double::read(NBT_File *fh)
{
	return fh->read(&number);
}

bool NBT_Tag_Double::write(NBT_File *fh)
{
	return fh->write(number);
}

std::string NBT_Tag_Double::serialize()
{
	std::stringstream strstr;
   strstr << this->className();

   if(named())
      strstr << "(" << this << "; " << name() << "; " <<  number << ")";
   else
      strstr << "(" << this << "; " << number << ")";
      
	return strstr.str();
}
