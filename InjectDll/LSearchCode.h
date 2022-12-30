#pragma once


typedef struct _FindCode
{
	UCHAR code[0x200];
	ULONG len;
	int offset;
	ULONG lastAddressOffset;
}FindCode, * PFindCode;

HMODULE GetGameWinBase(LPCSTR moduleName);
DWORD GetModuleLen(HMODULE hModule);
void initFindCodeStruct(PFindCode findCode, PCHAR code, ULONG_PTR offset, ULONG_PTR lastAddrOffset);

ULONG_PTR findAddressByCode(ULONG_PTR beginAddr, ULONG_PTR endAddr, PFindCode  findCode, ULONG numbers);

ULONG searchCode(char* moduleName, char* segmentName, char* code, int offset);