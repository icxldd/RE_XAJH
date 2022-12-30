#include "CMemory.h"
#include <stdio.h>

DWORD CMemory::ReadDword(PVOID nAddr, DWORD &nRet)
{
	if (IsBadReadPtr(nAddr, 4) == 0)
	{
		nRet = *(PDWORD)nAddr;
		return 4;
	}
	else {
		return 0;
	}
}

DWORD CMemory::ReadDword(DWORD nAddr, DWORD &nRet)
{
	if (IsBadReadPtr((PVOID)nAddr, 4) == 0)
	{
		nRet = *(PDWORD)nAddr;
		return 4;
	}
	else {
		return 0;
	}
}

DWORD CMemory::ReadDword(PVOID nAddr)
{
	if (IsBadReadPtr(nAddr, 4) == 0)
	{
		return *(PDWORD)nAddr;
	}
	else {
		char buf[100] = {0};
		sscanf(buf, "地址：%p不可读", nAddr);
		OutputDebugStringA(buf);
		return -1;
	}
}

DWORD CMemory::ReadDword(DWORD nAddr)
{
	if (IsBadReadPtr((PVOID)nAddr, 4) == 0)
	{
		return *(PDWORD)nAddr;
	}
	else {
		char buf[100] = { 0 };
		sscanf(buf, "地址：%p不可读", nAddr);
		OutputDebugStringA(buf);
		return -1;
	}
}





PVOID CMemory::CalcOffset(PVOID addr, DWORD offset)
{
	return (PVOID)(((DWORD)addr) + offset);
}
