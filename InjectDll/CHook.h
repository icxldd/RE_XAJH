#pragma once


//HOOK任意地址
void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress);


//MinHook例子
void MiniHookWxLogs();





struct HookContext
{
	DWORD ELF;
	DWORD EDI;
	DWORD ESI;
	DWORD EBP;
	DWORD ESP;
	DWORD EBX;
	DWORD EDX;
	DWORD ECX;
	DWORD EAX;
};


typedef void(_stdcall * InlineFunc)(HookContext*);

//HOOK任意地址(可以用反汇编引擎解析并调用被覆盖的代码 能HOOK任意地址 最优解)
bool HookAnyAddressByBeaEngine(DWORD pTarget, InlineFunc func);