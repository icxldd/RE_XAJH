#pragma once
#include "pch.h"



//HOOK��ȡ΢��������־
void HookGetLogs();

//��ȡ΢����־(�㺯��)
void GetLogs();


void __stdcall GetLogsCore(DWORD info1, DWORD info2, DWORD info3);



//700DC0
void HookCoreCallFor0x700DC0();
//��ȡ΢����־(�㺯��)
void GetLogsFor0x700DC0();
void __stdcall GetLogsCoreFor0x700DC0(DWORD** info1);




void __stdcall GetLogsByBeaEngine(HookContext *hookContent);


//ͨ�����������HOOK��־
void HookGetLogsByBeaEngine();