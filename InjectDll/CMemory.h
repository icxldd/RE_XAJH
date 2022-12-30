#pragma once
#include <windows.h>
class CMemory
{
public:
	//È¡4×Ö½Ú
	static DWORD ReadDword(PVOID nAddr, DWORD &nRet);
	static DWORD ReadDword(DWORD nAddr, DWORD &nRet);
	static DWORD ReadDword(PVOID nAddr);
	static DWORD ReadDword(DWORD nAddr);

	//¼ÆËãÆ«ÒÆ
	static PVOID CalcOffset(PVOID addr, DWORD offset);


};

