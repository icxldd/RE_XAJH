#pragma once


//HOOK�����ַ
void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress);


//MinHook����
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

//HOOK�����ַ(�����÷����������������ñ����ǵĴ��� ��HOOK�����ַ ���Ž�)
bool HookAnyAddressByBeaEngine(DWORD pTarget, InlineFunc func);