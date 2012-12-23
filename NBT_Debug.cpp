#include "NBT_Debug.h"
#include <cstdarg>
#include <cstdio>

void NBT_Debug_(const char *FILE, const char *FUNCTION, int LINE, const char *t, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	int msg_len = vsnprintf(0, 0, format, ap);
	va_end(ap);
	
	char *msg = new char[msg_len+1];
	if(!msg)
		return;
	
	msg[msg_len] = 0;
	
	va_start(ap, format);
	vsnprintf(msg, msg_len+1, format, ap);
	va_end(ap);
	
	printf("%s:%s:%i: %s: %s\n", FILE, FUNCTION, LINE, t, msg);
	
	delete[] msg;
}