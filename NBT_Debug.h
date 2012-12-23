#ifndef NBT_DEBUG_H_GUARD
#define NBT_DEBUG_H_GUARD

#ifdef DEBUG

void NBT_Debug_(const char *FILE, const char *FUNCTION, int LINE, const char *t, const char *format, ...);

#define NBT_Debug(a, b...) NBT_Debug_(__FILE__, __FUNCTION__, __LINE__, "D", a, ##b)
#define NBT_Warn(a, b...) NBT_Debug_(__FILE__, __FUNCTION__, __LINE__, "W", a, ##b)
#define NBT_Error(a, b...) NBT_Debug_(__FILE__, __FUNCTION__, __LINE__, "E", a, ##b)

#else

#define NBT_Debug(a, b...) ((void)a)
#define NBT_Warn(a, b...) ((void)a)
#define NBT_Error(a, b...) ((void)a)

#endif /* !defined DEBUG */

#endif /* NBT_DEBUG_H_GUARD */
