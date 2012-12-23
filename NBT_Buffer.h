#ifndef NBT_BUFFER_H_GUARD
#define NBT_BUFFER_H_GUARD

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

#include "NBT_Debug.h"

class NBT_Buffer
{
	public:
		NBT_Buffer();
		NBT_Buffer(void *data_, uint32_t len);
		~NBT_Buffer();
		
		NBT_Buffer *subBuffer();
		
		template <typename T> bool peek(T *out)
		{
			if(checkOffset(sizeof(T)))
			{
				*out = *((T*)(data_+offset_));
				return true;
			}
			
			return false;
		}

		template <typename T> bool read(T *out)
		{
			if(checkOffset(sizeof(T)))
			{
				*out = *((T*)(data_+offset_));
				offset_ += sizeof(T);
				return true;
			}
			
			return false;
		}

		template <typename T> bool read(T *out, uint32_t len)
		{
			if(checkOffset(sizeof(T) * len))
			{
				memcpy(out, data_ + offset_, sizeof(T)*len);
				offset_ += sizeof(T) * len;
				return true;
			}
			
			NBT_Debug("EOF in read len?");
			return false;
		}
		
		bool readShort(int16_t *out);
		bool readInt(int32_t *out);
		bool readLong(int64_t *out);
		
		bool readFloat(float *out);
		bool readDouble(double *out);
		
		bool peekShort(int16_t *out);
		
		bool eof() { return offset_ >= size_; }
		
		int32_t length() { return length_; }
		int32_t offset() { return offset_; }
		uint8_t *data() { return data_; }
		bool readonly() { return readonly_; }

		template <typename T> bool write(const T &in)
		{
			if(ensureSize(sizeof(T)))
			{
				*(data_+offset_) = in;
				offset_ += sizeof(T);
				if(offset_ > length_) // handle the case of seeking backwards, if that is ever implemented.
					length_ = offset_;

				return true;
			}

			return false;
		}

		template <typename T> bool write(T *in, uint32_t len)
		{
			if(ensureSize(sizeof(T) * len))
			{
				memcpy(data_+offset_, in, len);
				offset_ += sizeof(T) * len;
				if(offset_ > length_)
					length_ = offset_;

				return true;
			}

			return false;
		}

		bool writeShort(int16_t in);
		bool writeInt(int32_t in);
		bool writeLong(int64_t in);
		bool writeFloat(float in);
		bool writeDouble(double in);


	private:
		uint8_t *data_;
		int32_t size_;
		int32_t length_;
		int32_t offset_;
	
		bool readonly_;

		bool checkOffset(int32_t len)
		{
			/*NBT_Debug("%i - %i - %i = %i", length, offset, len, length - offset - len);*/
			return (offset_ + len <= size_);
		}

		// get the next power of two
		int32_t npot(int32_t in)
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
};


template <> inline bool NBT_Buffer::read<std::string>(std::string *out)
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

template <> inline bool NBT_Buffer::write<std::string>(const std::string &in)
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

#endif /* NBT_BUFFER_H_GUARD */
