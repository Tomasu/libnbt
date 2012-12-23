#include "NBT_Buffer.h"

#include <string>
#include "NBT_Debug.h"

NBT_Buffer::NBT_Buffer() : data_(0), size_(0), offset_(0), readonly_(false)
{
	// nothing
}

NBT_Buffer::NBT_Buffer(void *data, uint32_t len) : data_((uint8_t *)data), size_(len), offset_(0), readonly_(true)
{
	// nothing to do
}

NBT_Buffer::~NBT_Buffer()
{
	// also nothing to do
}

NBT_Buffer *NBT_Buffer::subBuffer()
{
	return new NBT_Buffer(((uint8_t*)data_)+offset_, size_ - offset_);
}

bool NBT_Buffer::readShort(int16_t *out)
{
	if(!read(out))
		return false;
	
	uint16_t *src = (uint16_t *)out;
	
	*out = *src << 8 | *src >> 8;
	
	return true;
}

bool NBT_Buffer::readInt(int32_t *out)
{
	if(!read(out))
		return false;
	
	uint32_t *src = (uint32_t *)out;
	
	*out = (*src >> 24) | ((*src & 0x00ff0000) >> 8) | ((*src & 0x0000ff00) << 8) | (*src << 24);
	return true;
}

bool NBT_Buffer::readLong(int64_t *out)
{
	if(!read(out))
		return false;
	
	uint64_t *src = (uint64_t *)out;
	
	*out = (*src >> 56) |
			 ((*src << 40) & 0x00ff000000000000) |
			 ((*src << 24) & 0x0000ff0000000000) |
			 ((*src << 8)  & 0x000000ff00000000) |
			 ((*src >> 8)  & 0x00000000ff000000) |
			 ((*src >> 24) & 0x0000000000ff0000) |
			 ((*src >> 40) & 0x000000000000ff00) |
			 (*src << 56);
			 
	return true;
}

bool NBT_Buffer::readFloat(float *out)
{
	return readInt((int32_t*)out);
}

bool NBT_Buffer::readDouble(double *out)
{
	return readLong((int64_t*)out);
}

bool NBT_Buffer::peekShort(int16_t *out)
{
	if(!peek(out))
		return false;
	
	uint16_t *src = (uint16_t *)out;
	
	*out = *src << 8 | *src >> 8;
	
	return true;
}

bool NBT_Buffer::writeShort(int16_t in)
{
	uint16_t out = in << 8 | in >> 8;
	return write(out);
}

bool NBT_Buffer::writeInt(int32_t in)
{
	uint32_t out = (in >> 24) | ((in & 0x00ff0000) >> 8) | ((in & 0x0000ff00) << 8) | (in << 24);
	return write(out);
}

bool NBT_Buffer::writeLong(int64_t in)
{
	uint64_t out = (in >> 56) |
			((in << 40) & 0x00ff000000000000) |
			((in << 24) & 0x0000ff0000000000) |
			((in << 8)  & 0x000000ff00000000) |
			((in >> 8)  & 0x00000000ff000000) |
			((in >> 24) & 0x0000000000ff0000) |
			((in >> 40) & 0x000000000000ff00) |
			(in << 56);

	return write(out);
}

bool NBT_Buffer::writeFloat(float in)
{
	return writeInt((int32_t)in);
}

bool NBT_Buffer::writeDouble(double in)
{
	return writeDouble((int64_t)in);
}

// try to ensure we always have enough space to write data
//  while trying to keep the number of realloc's down by
//  reallocating the buffer to incrementing powers of two,
//  starting at 4096.
bool NBT_Buffer::ensureSize(int32_t len)
{
	if(len <= 0)
		return true;

	if(offset_ + len >= size_)
	{
		int32_t new_size = npot(size_);
		if(new_size <= 4096)
			new_size = 4096;

		uint8_t *new_data = (uint8_t *)realloc(data_, new_size);
		if(!new_data)
			return false;

		data_ = new_data;
		size_ = new_size;
	}

	return true;
}

