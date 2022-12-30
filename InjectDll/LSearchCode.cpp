#include "pch.h"
#include "LSearchCode.h"

UCHAR charToHex(UCHAR* ch)
{

	//return *ch;

	unsigned char temps[2] = { 0 };
	for (int i = 0; i < 2; i++)
	{
		if (ch[i] >= '0' && ch[i] <= '9')
		{
			temps[i] = (ch[i] - '0');
		}
		else if (ch[i] >= 'A' && ch[i] <= 'F')
		{
			temps[i] = (ch[i] - 'A') + 0xA;
		}
		else if (ch[i] >= 'a' && ch[i] <= 'f')
		{
			temps[i] = (ch[i] - 'a') + 0xA;
		}
	}
	return ((temps[0] << 4) & 0xf0) | (temps[1] & 0xf);
}


/// <summary>
/// 
/// </summary>
/// <param name="findCode"></param>
/// <param name="code"></param>
/// <param name="offset">需要跳多少字节开始搜</param>
/// <param name="lastAddrOffset">搜到的结果做偏移操作</param>
void initFindCodeStruct(PFindCode findCode, PCHAR code, ULONG_PTR offset, ULONG_PTR lastAddrOffset)
{

	memset(findCode, 0, sizeof(FindCode));

	findCode->lastAddressOffset = lastAddrOffset;
	findCode->offset = offset;

	PCHAR pTemp = code;
	ULONG_PTR i = 0;
	for (i = 0; *pTemp != '\0'; i++)
	{
		if (*pTemp == '*' || *pTemp == '?')
		{
			findCode->code[i] = *pTemp;
			pTemp += 2;
			continue;
		}

		findCode->code[i] = charToHex((PUCHAR)pTemp);
		pTemp += 2;

	}

	findCode->len = i;
}


ULONG_PTR findAddressByCode(ULONG_PTR beginAddr, ULONG_PTR endAddr, PFindCode  findCode, ULONG numbers)
{
	ULONG64 j = 0;
	LARGE_INTEGER rtna = { 0 };

	for (ULONG_PTR i = beginAddr; i <= endAddr; i++)
	{

		if (!(IsBadReadPtr((PCHAR)(i), 1) == 0))
		{
			/*
			}
			if (!MmIsAddressValid((PVOID)i))
			{*/
			i = i & (~0xfff) + 0x1000 - 1;
			continue;
		}



		for (j = 0; j < numbers; j++)
		{
			FindCode  fc = findCode[j];
			ULONG_PTR tempAddress = i;

			UCHAR* code = (UCHAR*)(tempAddress + fc.offset);
			BOOLEAN isFlags = FALSE;

			for (ULONG_PTR k = 0; k < fc.len; k++)
			{
				//if (!MmIsAddressValid((PVOID)(code + k)))
				if (!(IsBadReadPtr((PCHAR)(code + k), 1) == 0))
				{
					isFlags = TRUE;
					break;
				}

				if (fc.code[k] == '*' || fc.code[k] == '?') continue;

				if (code[k] != fc.code[k])
				{
					isFlags = TRUE;
					break;
				}
			}

			if (isFlags) break;

		}

		//找到了
		if (j == numbers)
		{
			rtna.QuadPart = i;
			rtna.LowPart += findCode[0].lastAddressOffset;
			break;
		}

	}

	return rtna.QuadPart;
}


HMODULE GetGameWinBase(LPCSTR moduleName) {
	return (HMODULE)GetModuleHandleA(moduleName);
}


DWORD GetModuleLen(HMODULE hModule)
{
	PBYTE pImage = (PBYTE)hModule;
	PIMAGE_DOS_HEADER pImageDosHeader;
	PIMAGE_NT_HEADERS pImageNtHeader;
	pImageDosHeader = (PIMAGE_DOS_HEADER)pImage;
	if (pImageDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return 0;
	}
	pImageNtHeader = (PIMAGE_NT_HEADERS)&pImage[pImageDosHeader->e_lfanew];
	if (pImageNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		return 0;
	}
	return pImageNtHeader->OptionalHeader.SizeOfImage;
}


ULONG searchCode(char* moduleName, char* segmentName, char* code, int offset)
{
	FindCode fs[1] = { 0 };
	initFindCodeStruct(&fs[0], code, 0, offset);
	SIZE_T moduleBase = 0;
	HMODULE moduleHandle = GetGameWinBase(moduleName);
	ULONG size = GetModuleLen(moduleHandle);
	moduleBase = (SIZE_T)moduleHandle;
	if (!moduleBase)
	{
		return 0;
	}


	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)moduleBase;

	PIMAGE_NT_HEADERS pNts = (PIMAGE_NT_HEADERS)((PUCHAR)moduleBase + pDos->e_lfanew);

	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNts);

	PIMAGE_SECTION_HEADER pTemp = NULL;

	for (int i = 0; i < pNts->FileHeader.NumberOfSections; i++)
	{
		char bufName[9] = { 0 };
		memcpy(bufName, pSection->Name, 8);
		if (_stricmp(bufName, segmentName) == 0)
		{
			pTemp = pSection;
			break;
		}
		pSection++;
	}

	if (pTemp)
	{
		moduleBase = pSection->VirtualAddress + moduleBase;
		size = pSection->SizeOfRawData;
	}

	ULONG_PTR func = findAddressByCode(moduleBase, size + moduleBase, fs, 1);

	return func;
}