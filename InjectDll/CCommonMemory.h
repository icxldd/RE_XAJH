#pragma once
#include "windows.h"
#include "Utils.h"
using namespace std;
class CCommonMemory
{
public:
	string ReadMemoryUTA(wchar_t* address);
	




	template <typename elemType>
	elemType ReadMemory(ULONG_PTR address)
	{
		if (IsBadReadPtr((void*)address, sizeof(ULONG_PTR)) != 0)
		{
			return 0;
		}
		elemType buffer;
		buffer = *(elemType*)address;
		return buffer;
	}

	template <typename elemType>
	elemType ReadMemory(PULONG_PTR address)
	{
		if (IsBadReadPtr((void*)address, sizeof(PULONG_PTR)) != 0)
		{
			return 0;
		}
		elemType buffer;
		buffer = *(elemType*)address;
		return buffer;
	}


	template <typename elemType>
	BOOL WriteMemory(ULONG_PTR address, elemType value)
	{
		if (IsBadWritePtr((void*)address, sizeof(ULONG_PTR)) != 0)
		{
			return FALSE;
		}
		*(elemType*)address = value;
		return TRUE;
	}




};
