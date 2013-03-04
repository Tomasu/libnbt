#ifndef NBT_FILE_H_GUARD
#define NBT_FILE_H_GUARD

#include <cstdint>
#include <cstdio>
#include <string>
#include <cstring>

#include "NBT_Debug.h"

class NBT_File
{
   public:
      NBT_File();
      NBT_File(const std::string &filename, bool readonly = true, bool append = false);
      ~NBT_File();
      
      bool open();
      bool open(const std::string &filename);
      
      void close();
      
		bool readCompressedMode(uint32_t length);
		bool writeCompressedMode();
		
		bool endCompressedMode();
		bool clearCompressedMode();
		
      bool seek(int64_t offset, int whence = SEEK_SET);
      uint32_t tell()
		{
			if(compressedMode)
				return buffer_pos;
			
			return ftell(fh);
		}
      
		// maybe add a readBytes method and specialize read for the read* cases instead.
		// so read(mydouble) would work properly, then we can get rid of the readDouble method and its friends.
      
		bool read(uint8_t *out, uint32_t len = 1);
		bool read(uint16_t *out);
		bool read(uint32_t *out);
		bool read(uint32_t *out, uint32_t len);
		bool read(uint64_t *out);
		bool read(int8_t *out, uint32_t len = 1) { return read((uint8_t*)out, len); }
		bool read(int16_t *out) { return read((uint16_t*)out); }
		bool read(int32_t *out) { return read((uint32_t*)out); }
		bool read(int64_t *out) { return read((uint64_t*)out); }
		
		bool read(float *out);
		bool read(double *out);
		bool read(std::string &out);
      
		bool eof()
		{
			if(compressedMode)
				return buffer_len >= buffer_size;
			
			return feof(fh);
		}
		
		bool write(uint8_t *in, uint32_t len = 1);
		bool write(uint8_t in);
		bool write(uint16_t in);
		bool write(uint32_t in);
		bool write(uint32_t *in, uint32_t len);
		bool write(uint64_t in);
		
		bool write(int8_t *in, uint32_t len = 1) { return write((uint8_t*)in, len); }
		bool write(int8_t in) { return write((uint8_t)in); }
		bool write(int16_t in) { return write((uint16_t)in); }
		bool write(int32_t in) { return write((uint32_t)in); }
		bool write(int64_t in) { return write((uint64_t)in); }
		
		bool write(float in);
		bool write(double in);
		bool write(const std::string &in);
		
   private:
		std::string filename;
		bool readonly;
		bool append;
		enum {
			COMP_MODE_NONE = 0,
			COMP_MODE_READ,
			COMP_MODE_WRITE
		} compressedMode;
		
      FILE *fh;
		
      // buffer for handling compressed sections
		uint8_t *buffer;
		uint32_t buffer_size;
		uint32_t buffer_len;
		uint32_t buffer_pos;
		
		static int32_t npot(int32_t in)
		{
			in |= (in >> 1);
			in |= (in >> 2);
			in |= (in >> 4);
			in |= (in >> 8);
			in |= (in >> 16);
			in++;

			return in;
		}
		
		bool ensureSize(int32_t len);
		
		bool checkOffset(int32_t len)
		{
			/*NBT_Debug("%i - %i - %i = %i", length, offset, len, length - offset - len);*/
			return (buffer_pos + len <= buffer_len);
		}
		
		bool writeCompressedData();
};

inline bool NBT_File::read(uint8_t *out, uint32_t len)
{
	if(compressedMode)
	{
		if(checkOffset(sizeof(uint8_t) * len))
		{
			memcpy(out, buffer + buffer_pos, sizeof(uint8_t) * len);
			buffer_pos += sizeof(uint8_t) * len;
			return true;
		}
		
		return false; // eof?
	}
	
	if(!fh)
		return false;
	
	uint16_t ret = fread(out, sizeof(uint8_t), len, fh);
	if(ret != sizeof(uint8_t) * len)
		return false;
	
	return true;
}

inline bool NBT_File::read(uint16_t *out)
{
	uint8_t tmp[sizeof(*out)];
	if(!read(tmp, sizeof(*out)))
		return false;
	
	*out = tmp[0] << 8 | tmp[1];
	
	return true;
}

inline bool NBT_File::read(uint32_t *out)
{
	uint8_t tmp[sizeof(*out)];
	if(!read(tmp, sizeof(*out)))
		return false;
	
	uint32_t swap = (tmp[0] << 24) |
						(tmp[1] << 16) |
						(tmp[2] << 8) |
						tmp[3];
	*out = swap;
	
	//NBT_Debug("read: %08x -> %08x -> %u", *((uint32_t*)tmp), swap, swap);
	
	return true;
}

inline bool NBT_File::read(uint32_t *out, uint32_t len)
{
	bool ret = true;
	
	for(uint32_t i = 0; i < len && ret != false; i++)
	{
		ret = read(out+i);
	}
	
	return ret;
}

inline bool NBT_File::read(uint64_t *out)
{
	uint8_t tmp[sizeof(*out)];
	if(!read(tmp, sizeof(*out)))
		return false;
	
	*out = (uint64_t)tmp[0] << 56 | (uint64_t)tmp[1] << 48  |
			(uint64_t)tmp[2] << 40 | (uint64_t)tmp[3] << 32 |
			 (uint64_t)tmp[4] << 24 | (uint64_t)tmp[5] << 16 |
			 (uint64_t)tmp[6] << 8 | tmp[7];
	
	return true;
}

inline bool NBT_File::read(float *out)
{
	return read((uint32_t *)out);
}

inline bool NBT_File::read(double *out)
{
	return read((uint64_t *)out);
}

inline bool NBT_File::read(std::string &out)
{
	uint16_t len = 0;
	if(!read(&len))
		return false;
	
	//NBT_Debug("read len: %i", len);
	
	if(len == 0)
		return true;
	
	uint8_t *temp = (uint8_t *)malloc(len+1);
	if(!temp)
		return false;
	
	temp[len] = 0;
	
	if(!read(temp, len))
		return false;
	
	out.assign((char*)temp, len);
	
	free(temp);

	return true;
}

// write!

inline bool NBT_File::write(uint8_t *in, uint32_t len)
{
	if(compressedMode)
	{
		//NBT_Debug("writing %i bytes", len);
		if(ensureSize(len))
		{
			memcpy(buffer + buffer_pos, in, len);
			buffer_pos += len;
			if(buffer_pos > buffer_len)
				buffer_len = buffer_pos;
			
			return true;
		}
		
		return false;
	}
	
	uint32_t ret = fwrite(in, 1, len, fh);
	if(ret != len)
		return false;
	
	return true;
}

inline bool NBT_File::write(uint8_t in)
{
	//NBT_Debug("write char: %u", in);
	return write(&in, 1);
}

inline bool NBT_File::write(uint16_t in)
{
	uint8_t tmp[sizeof(in)], *ptr = (uint8_t *)&in;
	
	tmp[0] = ptr[1];
	tmp[1] = ptr[0];
	
	return write(tmp, sizeof(tmp));
}

inline bool NBT_File::write(uint32_t in)
{
	uint8_t tmp[sizeof(in)], *ptr = (uint8_t *)&in;
	
	//NBT_Debug("write int: %u", in);
	tmp[0] = ptr[3];
	tmp[1] = ptr[2];
	tmp[2] = ptr[1];
	tmp[3] = ptr[0];
	
	return write(tmp, sizeof(tmp));
}

inline bool NBT_File::write(uint32_t *in, uint32_t len)
{
	bool ret = true;
	
	for(uint32_t i = 0; i < len && ret != false; i++)
	{
		ret = write(in[i]);
	}
	
	return ret;
}

inline bool NBT_File::write(uint64_t in)
{
	uint8_t tmp[sizeof(in)], *ptr = (uint8_t *)&in;
	
	tmp[0] = ptr[7];
	tmp[1] = ptr[6];
	tmp[2] = ptr[5];
	tmp[3] = ptr[4];
	tmp[4] = ptr[3];
	tmp[5] = ptr[2];
	tmp[6] = ptr[1];
	tmp[7] = ptr[0];
	
	return write(tmp, sizeof(tmp));
}

inline bool NBT_File::write(float in)
{
	return write((uint32_t)in);
}

inline bool NBT_File::write(double in)
{
	return write((uint64_t)in);
}

inline bool NBT_File::write(const std::string &in)
{
	uint16_t len = in.length();

	if(!ensureSize(sizeof(len) + sizeof(int8_t) * len))
		return false;

	if(!write(len))
		return false;

	if(!write((uint8_t*)in.c_str(), len))
		return false;

	return true;
}

#endif /* NBT_FILE_H_GUARD */
