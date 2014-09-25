#include <zlib.h>
#include <cassert>
#include <cerrno>

#include "NBT_File.h"
#include "NBT_Debug.h"

// a sector in a chunk is 4k, and zlib prefers larger buffers, so give a multiple of 4k
#define INITIAL_BUFFER_SIZE (4096*8)

NBT_File::NBT_File() :
	readonly(true), append(false), compressedMode(COMP_MODE_NONE),
	fh(0), buffer(0), buffer_size(0), buffer_pos(0)
{
	
}

NBT_File::NBT_File(const std::string &filename, bool readonly, bool append) :
	filename(filename), readonly(readonly), append(append), compressedMode(COMP_MODE_NONE),
	fh(0), buffer(0), buffer_size(0), buffer_pos(0)
{
	
}

NBT_File::~NBT_File()
{
	close();
}

bool NBT_File::open()
{
	if(!filename.length())
		return false;
	
	return open(filename);
}

bool NBT_File::open(const std::string &filename)
{
	if(!filename.length())
		return false;
	
	const char *mode = "r";
	
	if(!readonly)
	{
		if(append)
			mode = "a+";
		else
			mode = "r+";
	}
	
	fh = fopen(filename.c_str(), mode);
	if(!fh)
	{
		if(errno == ENOENT)
		{
			fh = fopen(filename.c_str(), "w");
		}
		
		if(!fh)
		{
			NBT_Error("failed to open %s as %s: %s", filename.c_str(), mode, strerror(errno));
			return false;
		}
	}
	// ask stdio to buffer a bunch of data for us
	// should speed up reading, and we don't have to handle it ourselves!
	setvbuf(fh, 0, _IOFBF, 1024*1024*4); // 4MB!
	
	return true;
}

void NBT_File::close()
{
	if(buffer)
		free(buffer);

	buffer_size = 0;
	buffer_pos = 0;
	buffer_len = 0;
	buffer = 0;
	
	if(fh)
		fclose(fh);
	
	fh = 0;
}

bool NBT_File::readCompressedMode(uint32_t length, bool /*gzip*/)
{
	if(compressedMode)
		return false;
	
	//NBT_Debug("begin length: %i", length);
	
	// read compressed data into buffer
	uint8_t *compressedData = (uint8_t*)malloc(length);
	if(!compressedData)
		return false;
	
	uint32_t ret = fread(compressedData, 1, length, fh);
	if(ret != length)
	{
		free(compressedData);
		return false;
	}
	
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = 0;
	
	int zret = Z_OK;

	//if(!gzip)
	//	zret = inflateInit(&strm);
	//else
	zret = inflateInit2(&strm, 32+MAX_WBITS);
	if(zret != Z_OK)
	{
		NBT_Error("failed to initialize zlib");
		return false;
	}
	
	if(buffer_size < INITIAL_BUFFER_SIZE)
		buffer_size = INITIAL_BUFFER_SIZE;
	
	buffer = (uint8_t*)realloc(buffer, buffer_size);
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
		//NBT_Debug("buffer_len: %i, buffer_size: %i", buffer_len, buffer_size);
		
		strm.avail_out = buffer_size - buffer_len;
		strm.next_out = buffer + buffer_len;
		
		zret = inflate(&strm, Z_SYNC_FLUSH);
		assert(zret != Z_STREAM_ERROR);
//		NBT_Debug("zret: %i", zret);
		
		switch(zret)
		{
			case Z_NEED_DICT:
				NBT_Error("need dict?");
				ret = Z_DATA_ERROR;
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
//				NBT_Debug("zError: %i", zret);
				NBT_Error("decompression error: %s", zError(zret));
				(void)inflateEnd(&strm);
				return false;
		}
		
		if(zret != Z_BUF_ERROR)
		{
			// we had enough space in the buffer, so data was read into it
			// add the amount read into our buffer to our buffer_len variable.
			buffer_len += (buffer_size - buffer_len) - strm.avail_out;
			force_resize = false;
		}

		if(force_resize/* || buffer_len + INITIAL_BUFFER_SIZE > buffer_size*/)
		{
			uint32_t tmp_size = npot(buffer_size);
			uint8_t *tmp = (uint8_t *)realloc(buffer, tmp_size);
			if(!tmp)
			{
				NBT_Error("failed to allocate memory (%i bytes) for decompressed data", tmp_size);
				(void)inflateEnd(&strm);
				return false;
			}
			
			buffer_size = tmp_size;
			buffer = tmp;
		}
		
		//NBT_Debug("decompress avail_in: %i, avail_out: %i", strm.avail_in, strm.avail_out);
	} while(strm.avail_out == 0);
	
	inflateEnd(&strm);
	
	free(compressedData);
	
	//NBT_Debug("got %i bytes of uncompressed data", buffer_len);
	compressedMode = COMP_MODE_READ;
	
	//NBT_Debug("end");
	
	return true;
}

bool NBT_File::writeCompressedMode()
{
	if(compressedMode)
		return false;
	
	//NBT_Debug("enter writeCompressedMode");
	//NBT_Debug("begin");
	
	buffer_len = 0;
	
	if(!buffer || buffer_size < INITIAL_BUFFER_SIZE)
	{
		buffer = (uint8_t*)realloc(buffer, INITIAL_BUFFER_SIZE);
		if(!buffer)
		{
			NBT_Error("failed to allocate memory for compression buffer");
			return false;
		}
	}
		
	compressedMode = COMP_MODE_WRITE;
	
	//NBT_Debug("end");
	
	return true;
}

bool NBT_File::clearCompressedMode()
{
	if(!compressedMode)
	{
		NBT_Error("can't clear compressed mode if we're not in compressed mode");
		return false;
	}

	//NBT_Debug("");
	compressedMode = COMP_MODE_NONE;
	buffer_len = 0;
	buffer_pos = 0;
	
	return true;
}

bool NBT_File::endCompressedMode()
{
	if(!compressedMode)
	{
		NBT_Error("can't end compressed mode if we're not in compressed mode");
		return false;
	}
	
	//NBT_Debug("begin");
	
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
	
	//NBT_Debug("end");
	return ret;
}

bool NBT_File::writeCompressedData()
{
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = buffer_pos;
	strm.next_in = buffer;
	
	//NBT_Debug("write compressed data: %i bytes", buffer_pos);
	
	int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
	if(ret != Z_OK)
	{
		NBT_Error("failed to initialize zlib for compression");
		return false;
	}
	
	uint32_t comp_buffer_size = INITIAL_BUFFER_SIZE;
	uint32_t comp_buffer_len = 0;
	uint8_t *comp_buffer = (uint8_t*)realloc(0, comp_buffer_size);
	if(!comp_buffer)
	{
		NBT_Error("failed to allocate memory for compressed buffer");
		return false;
	}
	
	compressedMode = COMP_MODE_NONE;
	buffer_len = 0;
	buffer_pos = 0;
	
	do {
		bool force_resize = true;
		
		strm.avail_out = comp_buffer_size - comp_buffer_len;
		strm.next_out = comp_buffer + comp_buffer_len;
		
		ret = deflate(&strm, Z_FINISH);
		assert(ret != Z_STREAM_ERROR);
		
		//NBT_Debug("avail_in: %i, avail_out: %i, pos: %i", strm.avail_in, strm.avail_out, pos);
		
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
			//NBT_Debug("extending comp buffer pos from %i to %i", comp_buffer_len, (comp_buffer_size - comp_buffer_len) - strm.avail_out);
			comp_buffer_len += comp_buffer_size - strm.avail_out;
			force_resize = false;
		}
		
		if(force_resize || comp_buffer_len + INITIAL_BUFFER_SIZE < comp_buffer_size)
		{
			uint32_t tmp_size = npot(comp_buffer_size);
			//NBT_Debug("comp size: %u, tmp size: %u %i", comp_buffer_size, tmp_size, ret == Z_BUF_ERROR);
			uint8_t *tmp = (uint8_t *)realloc(comp_buffer, tmp_size);
			if(!tmp)
			{
				NBT_Error("failed to allocate memory (%i/%u) for compressed data", tmp_size, comp_buffer_size);
				deflateEnd(&strm);
				free(comp_buffer);
				return false;
			}
			
			comp_buffer_size = tmp_size;
			comp_buffer = tmp;
		}
		
	} while(strm.avail_out == 0);
	
	deflateEnd(&strm);
	
	uint32_t tmp_len = comp_buffer_len+1;
	//NBT_Debug("writing chunk len: %i", tmp_len);
	if(!write(tmp_len))
	{
		free(comp_buffer);
		NBT_Error("failed to write compressed data length to file");
		return false;
	}
	
	if(!write((uint8_t)2)) // force specific override
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
	
	last_write_buffer_len = comp_buffer_len+5;
	
	//NBT_Debug("compressed %i bytes to %i bytes", blen, comp_buffer_len);
	
	free(comp_buffer);
	
	return true;
}

bool NBT_File::seek(int64_t offset, int whence)
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
	
	//NBT_Debug("seeking to: %i", offset);
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
		
		//NBT_Debug("extending buffer from %i bytes to %i bytes (pos:%i)", buffer_size, new_size, buffer_pos);
		uint8_t *new_buffer = (uint8_t *)realloc(buffer, new_size);
		if(!new_buffer)
			return false;
		
		buffer = new_buffer;
		buffer_size = new_size;
	}
	
	return true;
}
