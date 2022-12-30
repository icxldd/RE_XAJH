#include "LLog.h"


void DebugPrint(char* fmt, ...)
{
	//__VA_ARGS__
	char sprint_buf[0x1000];
	va_list args;
	int n;
	va_start(args, fmt);
	n = vsprintf(sprint_buf, fmt, args);
	va_end(args);

	OutputDebugStringA(sprint_buf);

}
