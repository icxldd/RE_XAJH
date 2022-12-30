#include "CCommonMemory.h"

string CCommonMemory::ReadMemoryUTA(wchar_t* address)
{
	if (IsBadReadPtr((void*)address, sizeof(2)) != 0)
	{
		return "";
	}
	wstring str(address);
	return Utils::WstringToAnsi(str);
}