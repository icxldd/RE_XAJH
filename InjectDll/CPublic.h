#pragma once
#include "pch.h"
#include "CHook.h"




//获取WeChatWin的基地址
DWORD GetWeChatWinBase();



DWORD GetGameWinBase();

//DWORD GetModuleLen(HMODULE hModule);



void InitVectorHook();
void __stdcall InitVectorByBeaEngine(HookContext *hookContent);
