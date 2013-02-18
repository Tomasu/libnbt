#include <zlib.h>

#include "NBT_File.h"
#include "NBT_Debug.h"

// a sector in a chunk is 4k, and zlib prefers larger buffers, so give a multiple of 4k
#define INITIAL_BUFFER_SIZE (4096*4)

NBT_File::NBT_File() :
	readonly(true), append(false), compressedMode(false),
	fh(0), buffer(0), buffer_size(0), buffer_pos(0)
{
	
}

NBT_File::NBT_File(const std::string &filename, bool readonly, bool append) :
	filename(filename), readonly(readonly), append(append), compressedMode(false),
	fh(0), buffer(0), buffer_size(0), buffer_pos(0)
{
	
}

NBT_File::~NBT_File()
{
	
}

bool NBT_File::open()
{
	
}

bool NBT_File::open(const std::string &filename)
{
	
}

void NBT_File::close()
{
	
}

bool NBT_File::readCompressedMode(int length)
{
	if(compressedMode)
		return false;
	
	// read compressed data into buffer
	uint8_t *compressedData = malloc(length);
	if(!compressedData)
		return false;
	
	uint32_t ret = fread(compressedData, 1, length, fh);
	if(ret != length)
		return false;
	
	z_stream strm = {
		.zalloc = Z_NULL,
		.zfree = Z_NULL,
		.opaque = Z_NULL,
		.avail_in = 0,
		.next_in = 0
	};
	
	int ret = inflateInit(&strm);
	if(ret != Z_OK)
	{
		NBT_Error("failed to initialize zlib");
		return false;
	}
	
	if(buffer_size < INITIAL_BUFFER_SIZE)
		buffer_size = INITIAL_BUFFER_SIZE;
	
	buffer = realloc(buffer, buffer_size);
	if(!buffer)
	{
		NBT_Error("failed to allocate dest buffer");
		inflateEnd(&strm);
		return false;
	}
	
	buffer_pos = 0;
	buffer_len = 0;
	
	strm.avail_in = length;
	strm.next_in = (Bytef *)compressedData;
	
	do {
		bool force_resize = true;
		
		strm.avail_out = buffer_size - buffer_len;
		strm.next_out = buffer + buffer_len;
		
		ret = inflate(&strm, Z_NO_FLUSH);
		assert(ret != Z_STREAM_ERROR);
		
		switch(ret)
		{
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				NBT_Error("decompression error: %s", zError(ret));
				return false;
		}
		
		if(ret != Z_BUF_ERROR)
		{
			// we had enough space in the buffer, so data was read into it
			// add the amount read into our buffer to our buffer_len variable.
			buffer_len += (buffer_size - buffer_len) - strm.avail_out;
			force_resize = false;
		}

		if(force_resize || buffer_len + INITIAL_BUFFER_SIZE < buffer_size)
		{
			uint32_t tmp_size = npot(buffer_size);
			uint8_t *tmp = (uint8_t *)realloc(buffer, tmp_size);
			if(!tmp)
			{
				NBT_Error("failed to allocate memory for decompressed data");
				(void)inflateEnd(&strm);
				return false;
			}
			
			buffer_size = tmp_size;
			buffer = tmp;
		}
		
	} while(strm.avail_out == 0);
	
	inflateEnd(&strm);
	
	compressedMode = COMP_MODE_READ;
	
	return true;
}

bool NBT_File::writeCompressedMode()
{
	if(compressedMode)
		return false;
	
	buffer_len = 0;
	
	if(!buffer || buffer_size < INITIAL_BUFFER_SIZE)
	{
		buffer = realloc(buffer, INITIAL_BUFFER_SIZE);
		if(!buffer)
		{
			NBT_Error("failed to allocate memory for compression buffer");
			return false;
		}
	}
		
	compressedMode = COMP_MODE_WRITE;
	
	return true;
}

bool NBT_File::endCompressedMode()
{
	if(!compressedMode)
		return false;
	
	bool ret = false;
	
	if(compressedMode == COMP_MODE_READ)
	{
		ret = true;
	}
	else if(compressedMode == COMP_MODE_WRITE)
	{
		ret = writeCompressedData();
	}
	else
	{
		NBT_Error("unknown compressed mode >:(");
		ret = false;
	}
	
	compressedMode = COMP_MODE_NONE;
	buffer_len = 0;
	buffer_pos = 0;
	
	return ret;
}

bool NBT_File::writeCompressedData()
{
	z_stream strm = {
		.zalloc = Z_NULL,
		.zfree = Z_NULL,
		.opaque = Z_NULL,
		.avail_in = buffer_size,
		.next_in = buffer
	};
	
	int ret = deflateInit(&strm);
	if(ret != Z_OK)
	{
		NBT_Error("failed to initialize zlib for compression");
		return false;
	}
	
	uint32_t pos = 0;
	uint32_t comp_buffer_size = INITIAL_BUFFER_SIZE;
	uint32_t comp_buffer_len = 0;
	uint8_t *comp_buffer = realloc(0, comp_buffer_size);
	if(!comp_buffer)
	{
		NBT_Error("failed to allocate memory for compressed buffer");
		return false;
	}
	
	do {
		bool force_resize = true;
		
		strm.avail_out = comp_buffer_len - pos;
		strm.next_out = comp_buffer + pos;
		
		ret = deflate(&strm, Z_FINISH);
		assert(ret != Z_STREAM_ERROR);
		
		switch(ret)
		{
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				deflateEnd(&strm);
				NBT_Error("compression error: %s", zError(ret));
				free(comp_buffer);
				return false;
		}
		
		if(ret != Z_BUF_ERROR)
		{
			comp_buffer_len += (comp_buffer_size - comp_buffer_len) - strm.avail_out;
			force_resize = false;
		}
		
		if(force_resize || comp_buffer_len + INITIAL_BUFFER_SIZE < comp_buffer_size)
		{
			uint32_t tmp_size = npot(comp_buffer_size);
			uint8_t *tmp = (uint8_t *)realloc(comp_buffer, tmp_size);
			if(!tmp)
			{
				NBT_Error("failed to allocate memory for compressed data");
				deflateEnd(&strm);
				free(comp_buffer);
				return false;
			}
			
			comp_buffer_size = tmp_size;
			comp_buffer = tmp;
		}
		
	} while(strm.avail_out == 0);
	
	deflateEnd(&strm);
	
	// compression should be done here.
	
	if(!write(comp_buffer_len))
	{
		free(comp_buffer);
		NBT_Error("failed to write compressed data length to file");
		return false;
	}
	
	if(!write((uint8_t)1)) // force specific override
	{
		free(comp_buffer);
		NBT_Error("failed to write out compressed data type to file");
		return false;
	}
	
	if(fwrite(comp_buffer, 1, comp_buffer_len, fh) != comp_buffer_len)
	{
		NBT_Error("failed to write compressed data out to file :(");
		free(comp_buffer);
		return false;
	}
	
	compressedMode = COMP_MODE_NONE;
	buffer_len = 0;
	buffer_pos = 0;
	
	return true;
}

bool NBT_File::seek(int64_t offset, int whence = SEEK_SET)
{
	if(compressedMode)
	{
		int64_t new_pos = 0;
		switch(whence)
		{
			case SEEK_SET:
				new_pos = offset;
				break;
				
			case SEEK_CUR:
				new_pos = buffer_pos + offset;
				break;
				
			case SEEK_END:
				new_pos = buffer_len + offset;
				break;
		}
		
		if(new_pos < 0 || new_pos >= buffer_len)
			return false;
		
		return true;
	}
	
	if(fseek(fh, offset, whence) == 0)
		return true;
	
	return false;
}

// try to ensure we always have enough space to write data
//  in compressed mode while trying to keep the number of
//  realloc's down by reallocating the buffer to incrementing
//  powers of two, starting at 4096.
bool NBT_File::ensureSize(int32_t len)
{
	// only buffer data in compressed mode
	if(len <= 0 || !compressedMode)
		return true;
	
	if(buffer_pos + len >= buffer_size)
	{
		int32_t new_size = npot(buffer_size);
		if(new_size < 4096)
			new_size = 4096;
		
		uint8_t *new_buffer = (uint8_t *)realloc(buffer, new_size);
		if(!new_buffer)
			return false;
		
		buffer = new_buffer;
		buffer_size = new_size;
	}
	
	return true;
}
