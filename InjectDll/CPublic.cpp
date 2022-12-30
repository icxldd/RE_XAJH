#include "pch.h"
#include "CPublic.h"



//************************************
// 函数名称: GetWeChatWinBase
// 函数说明: 获取WeChatWin的基地址
// 作    者: 鬼手56
// 时    间: 2022/03/18
// 参    数: void
// 返 回 值: WeChatWin基地址 
//************************************
DWORD GetWeChatWinBase()
{
	return (DWORD)GetModuleHandleA("WeChatWin.dll");
}


DWORD GetGameWinBase() {
	return (DWORD)GetModuleHandleA("REStruct.exe");
}


//DWORD GetModuleLen(HMODULE hModule)
//{
//	PBYTE pImage = (PBYTE)hModule;
//	PIMAGE_DOS_HEADER pImageDosHeader;
//	PIMAGE_NT_HEADERS pImageNtHeader;
//	pImageDosHeader = (PIMAGE_DOS_HEADER)pImage;
//	if (pImageDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
//	{
//		return 0;
//	}
//	pImageNtHeader = (PIMAGE_NT_HEADERS)&pImage[pImageDosHeader->e_lfanew];
//	if (pImageNtHeader->Signature != IMAGE_NT_SIGNATURE)
//	{
//		return 0;
//	}
//	return pImageNtHeader->OptionalHeader.SizeOfImage;
//}

void InitVectorHook()
{
	HookAnyAddressByBeaEngine(GetGameWinBase() + InitVectorCallOffset, InitVectorByBeaEngine);
}
int g_vectorThis = 0;
void __stdcall InitVectorByBeaEngine(HookContext *hookContent)
{
	g_vectorThis = hookContent->ESI;
}
