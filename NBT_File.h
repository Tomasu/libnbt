#ifndef NBT_FILE_H_GUARD
#define NBT_FILE_H_GUARD

#include <cstdint>
#include <cstdio>
#include <string>

class NBT_File
{
   public:
      NBT_File();
      NBT_File(const std::string &filename, bool readonly = false, bool append = false);
      ~NBT_File();
      
      bool open();
      bool open(const std::string &filename);
      
      void close();
      
		bool readCompressedMode();
		bool writeCompressedMode();
		
		bool endCompressedMode();
		
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
		bool read(int16_t *out);
		bool read(int32_t *out);
		bool read(int64_t *out);
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
		
		bool write(int32_t in);
		bool write(int64_t in);
		bool write(float in);
		bool write(double in);
		bool write(std::string &in);
		
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
		size_t buffer_size;
		size_t buffer_len;
		pos_t buffer_pos;
		
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
		bool checkOffset(int32_t len);
		
		bool writeCompressedData();
};

inline bool NBT_Buffer::read(uint8_t *out, uint32_t len)
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
	
	int ret = fread(out, sizeof(uint8_t), len, fh);
	if(ret != sizeof(uint8_t) * len)
		return false;
	
	return true;
}

inline bool NBT_Buffer::read(int16_t *out)
{
	uint8_t tmp[sizeof(*out)];
	if(!read(&tmp, sizeof(*out)))
		return false;
	
	*out = tmp[1] | tmp[0];
	
	return true;
}

inline bool NBT_Buffer::read(int32_t *out)
{
	uint8_t tmp[sizeof(*out)];
	if(!read(&tmp, sizeof(*out)))
		return false;
	
	*out = tmp[3] | tmp[2] | tmp[1] | tmp[0];
	
	return true;
}

inline bool NBT_Buffer::read(int64_t *out)
{
	uint8_t tmp[sizeof(*out)];
	if(!read(&tmp, sizeof(*out)))
		return false;
	
	*out = tmp[7] | tmp[6] | tmp[5] | tmp[4] |
			 tmp[3] | tmp[2] | tmp[1] | tmp[0];
	
	return true;
}

inline bool NBT_Buffer::read(float *out)
{
	return read((uint32_t *)out);
}

inline bool NBT_Buffer::read(double *out)
{
	return read((uint64_t *)out);
}

inline bool NBT_Buffer::read(std::string &out)
{
	int16_t len = 0;
	if(!readShort(&len))
		return false;
	
	//NBT_Debug("read len: %i", len);
	
	if(len <= 0)
		return true;
	
	char *temp = (char *)malloc(len+1);
	if(!temp)
		return false;
	
	temp[len] = 0;
	
	if(!read(temp, len))
		return false;
	
	out->assign(temp, len);
	
	free(temp);

	return true;
}

// write!

inline bool NBT_Buffer::write(uint8_t *in, uint32_t len)
{
	if(compressedMode)
	{
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
	
	int ret = fwrite(in, 1, len, fh);
	if(ret != len)
		return false;
	
	return true;
}

inline bool NBT_File::write(uint8_t in)
{
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
	
	tmp[0] = ptr[3];
	tmp[1] = ptr[2];
	tmp[2] = ptr[1];
	tmp[3] = ptr[0];
	
	return write(tmp, sizeof(tmp));
}

inline bool NBT_File::write(int64_t in)
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

inline bool NBT_Buffer::write(const std::string &in)
{
	int16_t len = in.length();

	if(!ensureSize(sizeof(len) + sizeof(int8_t) * len))
		return false;

	if(!writeShort(len))
		return false;

	if(!write(in.c_str(), len))
		return false;

	return true;
}

#endif /* NBT_FILE_H_GUARD */
