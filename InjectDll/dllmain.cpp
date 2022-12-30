// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "CGetLogs.h"
#include <direct.h>
#include "CFunctions.h"
#include "CHook.h"


extern "C" __declspec(dllexport) int add(int a, int b);


int add(int a, int b)
{
	return a + b;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		OutputDebugStringA("WxDemo注入成功");

		HookGetLogs();
		//StartVoipChat2((wchar_t*)L"wxid_fineonxis3f012", 1);

		//MiniHookWxLogs();


		//HookGetLogsByBeaEngine();

	}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

