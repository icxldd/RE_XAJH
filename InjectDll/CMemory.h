#pragma once
#include <windows.h>
class CMemory
{
public:
	//ȡ4�ֽ�
	static DWORD ReadDword(PVOID nAddr, DWORD &nRet);
	static DWORD ReadDword(DWORD nAddr, DWORD &nRet);
	static DWORD ReadDword(PVOID nAddr);
	static DWORD ReadDword(DWORD nAddr);

	//����ƫ��
	static PVOID CalcOffset(PVOID addr, DWORD offset);


};

