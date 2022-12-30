#include "pch.h"
#include "CGetLogs.h"
#include "CHook.h"
#include <strsafe.h>


//跳转的地址
DWORD g_logJmpAddr = GetWeChatWinBase() + WxHookGetLog + 5;
DWORD g_logFor0x700DC0JmpAddr = GetWeChatWinBase() + Offset0x700DC0 + 5;



//************************************
// 函数名称: HookGetLogs
// 函数说明: HOOK获取微信运行日志
// 作    者: 鬼手56
// 时    间: 2022/03/18
// 参    数: 
// 返 回 值:  
//************************************
void HookGetLogs()
{
	HookAnyAddress(GetWeChatWinBase() + WxHookGetLog, GetLogs);
}




//************************************
// 函数名称: GetLogs
// 函数说明: 获取微信运行日志的裸函数 
// 作    者: 鬼手56
// 时    间: 2022/03/18
// 参    数: 
// 返 回 值:  
//************************************
__declspec(naked) void GetLogs()
{

	__asm
	{

		//1.保存当前的寄存器环境
		pushad;			//+0x20

		push[esp + 0x8 + 0x20];		//+4
		push[esp + 0xC + 0x20 + 0x4];	//+4
		push[esp + 0x10 + 0x20 + 0x8];
		call GetLogsCore;

		//恢复寄存器环境

		popad;


		//1.补充被覆盖的代码
		push ebp;
		mov ebp, esp;
		//and esp, 0xFFFFFFF8;

		//跳转回HOOK地址的下一条
		jmp g_logJmpAddr;
	}


}

void __stdcall GetLogsCore(DWORD info1, DWORD info2, DWORD info3)
{

	string sInfo1 = (char *)info1;
	string sInfo2 = (char *)info2;
	string sInfo3 = (char *)info3;


	string logs;
	logs.append("Wxlogs:");
	logs.append(sInfo1);
	logs.append("---");
	logs.append(sInfo2);
	logs.append("---");
	logs.append(sInfo3);

	//输出日志
	OutputDebugStringA(logs.c_str());

}





void HookCoreCallFor0x700DC0()
{
	HookAnyAddress(GetWeChatWinBase() + Offset0x700DC0, GetLogsFor0x700DC0);
}

__declspec(naked) void GetLogsFor0x700DC0()
{
	__asm
	{

		//1.保存当前的寄存器环境
		pushad;			//+0x20
		//int 3;

		push[esp + 0x8 + 0x20];		//+4
		call GetLogsCoreFor0x700DC0;

		//恢复寄存器环境

		popad;


		//1.补充被覆盖的代码
		mov ebp, esp;
		mov eax, dword ptr ss : [ebp + 8] ;

		//跳转回HOOK地址的下一条
		jmp g_logFor0x700DC0JmpAddr;
	}

}

void __stdcall GetLogsCoreFor0x700DC0(DWORD **info1)
{
	if (!IsBadReadPtr(info1, 4))
	{
		int const arraysize = 300;
		TCHAR pszDest[arraysize];
		size_t cbDest = arraysize * sizeof(TCHAR);
		auto infoPoint = *info1;
		if (infoPoint == NULL)
		{
			LPCTSTR pszFormat = TEXT("WxlogsFor0x700DC0:p1:%p p2:%p");

			StringCbPrintf(pszDest, cbDest, pszFormat, info1, infoPoint);
			OutputDebugStringW(pszDest);
		}
		else 
		{

			LPCTSTR pszFormat = TEXT("WxlogsFor0x700DC0:p1:%p p2:%p str:%s");

			StringCbPrintf(pszDest, cbDest, pszFormat, info1, infoPoint, infoPoint);
			OutputDebugStringW(pszDest);
		}
		
		//wstring sInfo1 = (wchar_t *)(*info1);
		//wstring logs;
		//logs.append(L"WxlogsFor0x700DC0:");
		//logs.append(sInfo1);

		////输出日志
		//OutputDebugStringW(logs.c_str());

		/*string sInfo1 = (char *)(*info1);
		string logs;
		logs.append("WxlogsFor0x700DC0:");
		logs.append(sInfo1);
		OutputDebugStringA(logs.c_str());*/
	}


}

void __stdcall GetLogsByBeaEngine(HookContext *hookContent)
{
	if (!IsBadReadPtr((void *)hookContent->EAX, 4))
	{
		//取日志的内容
		string content = (char *)hookContent->EAX;

		if (content.find("2022-3-19") != string::npos)
		{
			OutputDebugStringA(("BeaHookLog:" + content).c_str());
		}

	}

}

//************************************
// 函数名称: HookGetLogsByBeaEngine
// 函数说明: 通过反汇编引擎HOOK日志
// 作    者: 鬼手56
// 时    间: 2022/03/19
// 参    数: 
// 返 回 值:  
//************************************
void HookGetLogsByBeaEngine()
{
	//HookAnyAddressByBeaEngine(GetWeChatWinBase() + WxMiniHookGetLog, GetLogsByBeaEngine);
	HookAnyAddressByBeaEngine(GetWeChatWinBase() + WxHookGetLog, GetLogsByBeaEngine);
}
