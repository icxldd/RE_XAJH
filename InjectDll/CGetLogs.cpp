#include "pch.h"
#include "CGetLogs.h"
#include "CHook.h"
#include <strsafe.h>


//��ת�ĵ�ַ
DWORD g_logJmpAddr = GetWeChatWinBase() + WxHookGetLog + 5;
DWORD g_logFor0x700DC0JmpAddr = GetWeChatWinBase() + Offset0x700DC0 + 5;



//************************************
// ��������: HookGetLogs
// ����˵��: HOOK��ȡ΢��������־
// ��    ��: ����56
// ʱ    ��: 2022/03/18
// ��    ��: 
// �� �� ֵ:  
//************************************
void HookGetLogs()
{
	HookAnyAddress(GetWeChatWinBase() + WxHookGetLog, GetLogs);
}




//************************************
// ��������: GetLogs
// ����˵��: ��ȡ΢��������־���㺯�� 
// ��    ��: ����56
// ʱ    ��: 2022/03/18
// ��    ��: 
// �� �� ֵ:  
//************************************
__declspec(naked) void GetLogs()
{

	__asm
	{

		//1.���浱ǰ�ļĴ�������
		pushad;			//+0x20

		push[esp + 0x8 + 0x20];		//+4
		push[esp + 0xC + 0x20 + 0x4];	//+4
		push[esp + 0x10 + 0x20 + 0x8];
		call GetLogsCore;

		//�ָ��Ĵ�������

		popad;


		//1.���䱻���ǵĴ���
		push ebp;
		mov ebp, esp;
		//and esp, 0xFFFFFFF8;

		//��ת��HOOK��ַ����һ��
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

	//�����־
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

		//1.���浱ǰ�ļĴ�������
		pushad;			//+0x20
		//int 3;

		push[esp + 0x8 + 0x20];		//+4
		call GetLogsCoreFor0x700DC0;

		//�ָ��Ĵ�������

		popad;


		//1.���䱻���ǵĴ���
		mov ebp, esp;
		mov eax, dword ptr ss : [ebp + 8] ;

		//��ת��HOOK��ַ����һ��
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

		////�����־
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
		//ȡ��־������
		string content = (char *)hookContent->EAX;

		if (content.find("2022-3-19") != string::npos)
		{
			OutputDebugStringA(("BeaHookLog:" + content).c_str());
		}

	}

}

//************************************
// ��������: HookGetLogsByBeaEngine
// ����˵��: ͨ�����������HOOK��־
// ��    ��: ����56
// ʱ    ��: 2022/03/19
// ��    ��: 
// �� �� ֵ:  
//************************************
void HookGetLogsByBeaEngine()
{
	//HookAnyAddressByBeaEngine(GetWeChatWinBase() + WxMiniHookGetLog, GetLogsByBeaEngine);
	HookAnyAddressByBeaEngine(GetWeChatWinBase() + WxHookGetLog, GetLogsByBeaEngine);
}
