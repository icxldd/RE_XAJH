#pragma once
#include "pch.h"
#include "CHook.h"




//��ȡWeChatWin�Ļ���ַ
DWORD GetWeChatWinBase();



DWORD GetGameWinBase();

//DWORD GetModuleLen(HMODULE hModule);



void InitVectorHook();
void __stdcall InitVectorByBeaEngine(HookContext *hookContent);
