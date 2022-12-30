#pragma once
#include "pch.h"



//HOOK获取微信运行日志
void HookGetLogs();

//获取微信日志(裸函数)
void GetLogs();


void __stdcall GetLogsCore(DWORD info1, DWORD info2, DWORD info3);



//700DC0
void HookCoreCallFor0x700DC0();
//获取微信日志(裸函数)
void GetLogsFor0x700DC0();
void __stdcall GetLogsCoreFor0x700DC0(DWORD** info1);




void __stdcall GetLogsByBeaEngine(HookContext *hookContent);


//通过反汇编引擎HOOK日志
void HookGetLogsByBeaEngine();