#include "pch.h"
#include "CFunctions.h"
#include "AnyCall.h"
#include <vector>
#include <TlHelp32.h>
#include "ntinfo.h"
#include <windows.h>

// list all PIDs and TIDs
#include <tlhelp32.h>
#include <Psapi.h>
#include "Resource.h"
#include "YSignatureCode.h"
#include "CRETree.h"

#include "CCommonMemory.h"
//************************************
// 函数名称: StartVoipChat
// 函数说明: 语音/视频聊天
// 作    者: 鬼手56
// 时    间: 2022/03/18
// 参    数: wxid, dwType dwType 0视频通话 1语音通话
// 返 回 值: void
//************************************
void StartVoipChat(wchar_t* wxid, BOOL dwType)
{
	//获取WeChatWin的基地址
	DWORD dwWeChatWin = GetWeChatWinBase();

	DWORD dwCall1 = dwWeChatWin + WxStartVoipCall1;
	DWORD dwCall2 = dwWeChatWin + WxStartVoipCall2;
	DWORD dwCall3 = dwWeChatWin + WxStartVoipCall3;
	DWORD dwEcx = dwWeChatWin + WxStartVoipEcx;


	//微信结构体的指针
	GenericData pWxid(wxid);
	char* asmWxid = (char*)&pWxid.pstr;
	char buff[0xC] = { 0 };


	__asm
	{
		//int 3;			//x64dbg软件断点(一定要先附加调试器) 

		pushad;
		mov eax, asmWxid;
		push eax;
		push 0x0;
		lea ecx, buff;
		call dwCall1;

		push dwType;
		lea eax, buff;
		sub esp, 0xC;
		mov ecx, esp;
		push eax;
		call dwCall2;
		mov esi, dwEcx;
		mov ecx, esi;
		call dwCall3;
		popad;
	}






}

void StartVoipChat2(wchar_t* wxid, BOOL dwType)
{
	//获取WeChatWin的基地址
	DWORD dwWeChatWin = GetWeChatWinBase();

	DWORD dwCall3 = dwWeChatWin + WxStartVoipCall3;
	DWORD dwEcx = dwWeChatWin + WxStartVoipEcx;


	//要构造的栈的数据
	DWORD args[4] = { 0 };


	//用来构造微信ID的指针
	DWORD WxidData[0xD0] = { 0 };
	WxidData[0] = (DWORD)wxid;
	WxidData[1] = wcslen(wxid);
	WxidData[2] = wcslen(wxid) * 2;
	WxidData[3] = 0;
	WxidData[4] = 0;


	//0FACE850
	//0FACE864
	//0FACE864
	DWORD buffer[0xD0] = { 0 };
	buffer[0] = (DWORD)WxidData;
	buffer[1] = buffer[0] + 0x14;
	buffer[2] = buffer[0] + 0x14;



	args[0] = (DWORD)WxidData;
	args[1] = (DWORD)buffer[1];
	args[2] = (DWORD)buffer[1];
	args[3] = dwType;

	CopyDataInvokeFun(args, 0x10, dwCall3, dwEcx);

}




//************************************
// 函数名称: SendWxTextMsg
// 函数说明: 发送文本消息
// 作    者: 鬼手56
// 时    间: 2022/03/19
// 参    数: wxid, msg
// 返 回 值: void
//************************************
void SendWxTextMsg(wchar_t* wxid, wchar_t* msg)
{
	//获取发送文本消息call的地址
	DWORD dwCall = GetWeChatWinBase() + WxSendTextMsg;


	//构造数据
	GenericData sWxid(wxid);
	GenericData sMsg(msg);

	char* pWxid = (char*)&sWxid.pstr;
	char* pMsg = (char*)&sMsg.pstr;


	//被艾特人的缓冲区
	char buff[0x10] = { 0 };

	//ECX的缓冲区 这里必须给到足够大
	char EcxBuff[0x1000] = { 0 };



	__asm
	{
		lea eax, buff;
		push 0x1;
		push eax;
		mov edi, pMsg;
		push edi;
		mov edx, pWxid;
		lea ecx, EcxBuff;
		call dwCall;
		add esp, 0xC;
	}



}

//************************************
// 函数名称: SendWxTextMsgDemo
// 函数说明: 发送文本消息调用示例
// 作    者: 鬼手56
// 时    间: 2022/03/19
// 参    数: wxid, msg
// 返 回 值:  
//************************************
void SendWxTextMsgDemo(wchar_t* wxid, wchar_t* msg)
{
	//获取发送文本消息call的地址
	DWORD dwCall = GetWeChatWinBase() + WxSendTextMsg;


	//构造数据
	GenericData sWxid(wxid);
	GenericData sMsg(msg);

	char* pWxid = (char*)&sWxid.pstr;
	char* pMsg = (char*)&sMsg.pstr;


	//被艾特人的缓冲区
	char buff[0x10] = { 0 };

	//ECX的缓冲区 这里必须给到足够大
	char EcxBuff[0x1000] = { 0 };

	DWORD dwNum = 1;

	//这里是示例 不能成功调用
	AnyCall::invokeFastcall<int>(EcxBuff, pWxid, (void*)dwCall, pMsg, buff, dwNum);
}



//************************************
// 函数名称: CopyDataInvokeFun
// 函数说明: 拷贝数据到堆栈中并且调用函数
// 作    者: 鬼手56
// 时    间: 2022/03/19
// 参    数: data, datasize, dwCall, dwEcx
// 返 回 值: BOOL 是否调用成功
//************************************
BOOL CopyDataInvokeFun(void* data, DWORD datasize, DWORD dwCall, DWORD dwEcx)
{
	BOOL bRet = 0;

	__asm
	{
		pushad;
		pushfd;
		cld;					//让串操作指令的拷贝顺序颠倒(正向拷贝)
		sub esp, datasize;
		mov esi, data;			//从data中拷贝数据
		lea edi, [esp];			//拷贝到堆栈
		mov ecx, datasize;
		rep movsb;				//从后往前拷贝

		mov ecx, dwEcx;			//赋值ECX指针
		call dwCall;			//调用call
		mov bRet, eax;			//获取返回值

		popfd;
		popad;
	}

	return bRet;
}


void ReadGlobalData()
{

	DWORD ecx = *(PDWORD)(GetGameWinBase() + GlobalVarData);
	//CString str;
	//str.Format(L"%x", ecx);
	////str.Format(L"%d", *val);


	//AfxMessageBox(str.GetBuffer());

	//00FD6A96 | C787 D4000000 15160500   | mov dword ptr ds:[edi+D4],51615                                                   |
	PDWORD val = (PDWORD)(ecx + 0xd4);

	CString str2;
	str2.Format(L"address:%p,val:%d", val, *val);
	//str.Format(L"%d", *val);


	AfxMessageBox(str2.GetBuffer());


}
DWORD GetThreadStartAddress(HANDLE processHandle, HANDLE hThread) {
	/* rewritten from https://github.com/cheat-engine/cheat-engine/blob/master/Cheat%20Engine/CEFuncProc.pas#L3080 */
	DWORD used = 0, ret = 0;
	DWORD stacktop = 0, result = 0;

	MODULEINFO mi;

	GetModuleInformation(processHandle, GetModuleHandle(L"kernel32.dll"), &mi, sizeof(mi));
	stacktop = (DWORD)GetThreadStackTopAddress_x86(processHandle, hThread);


	CloseHandle(hThread);

	if (stacktop) {

		DWORD* buf32 = new DWORD[4096];

		if (ReadProcessMemory(processHandle, (LPCVOID)(stacktop - 4096), buf32, 4096, NULL)) {
			for (int i = 4096 / 4 - 1; i >= 0; --i) {
				if (buf32[i] >= (DWORD)mi.lpBaseOfDll && buf32[i] <= (DWORD)mi.lpBaseOfDll + mi.SizeOfImage) {
					result = stacktop - 4096 + i * 4;
					break;
				}

			}
		}

		delete buf32;
	}

	return result;
}
std::vector<DWORD> threadList(DWORD pid) {
	/* solution from http://stackoverflow.com/questions/1206878/enumerating-threads-in-windows */
	std::vector<DWORD> vect = std::vector<DWORD>();
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (h == INVALID_HANDLE_VALUE)
		return vect;

	THREADENTRY32 te;
	te.dwSize = sizeof(te);
	if (Thread32First(h, &te)) {
		do {
			if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) +
				sizeof(te.th32OwnerProcessID)) {


				if (te.th32OwnerProcessID == pid) {
					printf("PID: %04d Thread ID: 0x%04x\n", te.th32OwnerProcessID, te.th32ThreadID);
					vect.push_back(te.th32ThreadID);
				}

			}
			te.dwSize = sizeof(te);
		} while (Thread32Next(h, &te));
	}

	return vect;
}
int GetThreadStartAddress_SSY(int intThreadNo) {
	HANDLE hProcHandle = NULL;
	hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
	std::vector<DWORD> threadId = threadList(GetCurrentProcessId());
	int stackNum = 0;
	for (auto it = threadId.begin(); it != threadId.end(); ++it) {
		if (stackNum == intThreadNo)
		{
			HANDLE threadHandle = OpenThread(THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, FALSE, *it);
			DWORD threadStartAddress = GetThreadStartAddress(hProcHandle, threadHandle);
			return threadStartAddress;
		}
		stackNum++;
	}
	return 0;
}
extern int g_vectorThis;
void EachVectorData()
{


	//DWORD dd = (DWORD)GetThreadStartAddress_SSY(0);
	//DWORD l1 = *(PDWORD)(dd - 0x000002E8);
	//DWORD l2 = *(PDWORD)(l1 + 0x30);
	//DWORD l3 = *(PDWORD)(l2 + 0x1c);

	/*
	ecx
00555A7E | 8B8F 54010000            | mov ecx,dword ptr ds:[edi+154]                                                    |
16
[edi+154]+0 = 首地址
[edi+154]+4 = 结束地址
	*/
	CString str;
	DWORD ecx = g_vectorThis;
	PDWORD* vector_item = *(PDWORD**)(ecx + 0x154);
	//str.Format(L"[InjectDLL]ecx:%x start address:%p,end address:%p", ecx, adrees, end_adrees);
	PDWORD adrees = *vector_item;
	PDWORD end_adrees = *(vector_item + 1);


	OutputDebugString(str.GetBuffer());

	while (true)
	{
		//每项16字节
		int id = *adrees;
		PDWORD name = *(PDWORD*)(adrees + 1);
		int level = *(adrees + 3);


		str.Format(L"[InjectDLL]id:%d,level:%d,name:%s", id, level, name);

		OutputDebugString(str.GetBuffer());
		adrees += 4;
		if (adrees >= end_adrees)
		{
			break;
		}
	}

	//str.Format(L"ecx:%x start address:%p,end address:%p", ecx, adrees, end_adrees);
	////str.Format(L"%x", l3);
	//AfxMessageBox(str.GetBuffer());
}

void EachListData()
{
	/*
	55 8B EC 56 8B F1 56 E8 ?? ?? ?? ?? 8B 4D 08 83 C4 04 50 8B 06 FF 30 E8 ?? ?? ?? ?? 8B 45 08 5E 5D C2 04 00
	*/

	string sTeZm = "558BEC568BF156E8????????8B4D0883C404508B06FF30E8????????8B45085E5DC20400";
	HMODULE module = (HMODULE)GetGameWinBase();
	DWORD nModuleSize = 0;// GetModuleLen(module);
	YSignatureCode CYSignatureCode;
	PVOID adress = CYSignatureCode.search((const void*)module, nModuleSize, sTeZm);
	DWORD n_ecx = *(PDWORD)(((DWORD)module) + WorldOffset);
	n_ecx = *(PDWORD)(n_ecx + ListPageThisOffset);
	n_ecx = *(PDWORD)(n_ecx + 0x154);
	PDWORD pListAddress;

	__asm
	{
		pushad;
		mov ecx, n_ecx;
		lea eax, pListAddress;
		push eax;
		call adress;
		popad;
	}
	PDWORD nStartAddress = *(PDWORD*)pListAddress;
	PDWORD nCurrentAddress = *(PDWORD*)pListAddress;
	CString str;
	while (true)
	{


		//+0下一个   +4上一个   +8....数据

		DWORD id = *(PDWORD)(nCurrentAddress + 2);
		PWCHAR name = *(PWCHAR*)(nCurrentAddress + 3);
		//DWORD daqu = *(PDWORD)(nNext + 4);
		DWORD level = *(PDWORD)(nCurrentAddress + 5);
		str.Format(L"[InjectDLL]id:%d,level:%d,name:%s,currentAdress:%p,pListAddress:%p", id, level, name, nCurrentAddress, pListAddress);
		OutputDebugString(str.GetBuffer());

		nCurrentAddress = *(PDWORD*)nCurrentAddress;
		if (*nCurrentAddress == (DWORD)pListAddress)//遍历完了
		{
			break;
		}

	}






	//CString str2;
	//str2.Format(L"val:%p", adress);

	//AfxMessageBox(str2.GetBuffer());


}


#define necx [ebp + 0x1c + 0x20+0x4]
#define address6 [ebp + 0x18 + 0x20+0x4]
#define address5 [ebp + 0x14 + 0x20+0x4]
#define address4 [ebp + 0x10 + 0x20+0x4]
#define address3 [ebp + 0xc + 0x20+0x4]
#define address2 [ebp + 0x8 + 0x20+0x4]
#define address1 [ebp + 0x4 + 0x20+0x4]


void __stdcall EachMapDataCore2(PDWORD pObj)
{
	CString str;

	DWORD id = *(PDWORD)(pObj);
	PWCHAR name = *(PWCHAR*)(pObj + 1);
	DWORD level = *(PDWORD)(pObj + 3);
	str.Format(L"[InjectDLL]id:%d,level:%d,name:%s", id, level, name);
	OutputDebugString(str.GetBuffer());
}

__declspec(naked) void __stdcall EachMapDataCore()
{
	//7个参数	28h



	__asm {
		pushad;//0x20
		push    ebp;//0x4
		mov     ebp, esp;



		//push[ebp + 0x1c + 0x20];//n_ecx
		//push[ebp + 0x18 + 0x20];//6
		//push[ebp + 0x14 + 0x20];//5
		//push[ebp + 0x10 + 0x20];//4
		//push[ebp + 0xc + 0x20];//3
		//push[ebp + 0x8 + 0x20];//2
		//push[ebp + 0x4 + 0x20];//1
		push necx;
		push address6;
		push address5;
		push address4;
		push address3;
		push address2;
		push address1;
		call Core;


		mov     esp, ebp;
		pop     ebp;
		popad;
		retn	01ch;
	};

	__asm {
	Core:
		pushad;
		push    ebp;
		mov     ebp, esp;
		sub     esp, 0Ch;

		mov     esi, necx;
		push    4;
		lea     ecx, [ebp - 4];
		//xoredi, edi;
		mov edi, 0;
		call    address4;
		mov     ecx, [esi + 150h];
		lea     eax, [ebp - 4];
		push    eax;
		call    address2;
		mov     ecx, [esi + 150h];
		lea     eax, [ebp - 0ch];
		push    eax;
		call    address3;
		push    eax;
		lea     ecx, [ebp - 4];
		call    address1;
		test    al, al;
		jz      short loc_C14FE0;

	loc_C14F96:
		; 这里是关键代码;
		cmp     edi, 5;
		//jnz     short loc_C14FBB;
		lea     ecx, [ebp - 4];
		call    address6;
		;// mov[eax + 4], 4;
		add eax, 4;
		push eax;
		call EachMapDataCore2;

	loc_C14FBB:
		lea     ecx, [ebp - 4];
		call    TreeJiaJia;
		mov esi, necx;
		mov     ecx, [esi + 150h];
		lea     eax, [ebp - 0ch];
		push    eax;
		inc     edi;
		call    address3;
		push    eax;
		lea     ecx, [ebp - 4];
		call    address1;
		test    al, al;
		jnz     short loc_C14F96;

	loc_C14FE0:
		mov     esp, ebp;
		pop     ebp;
		popad;
		retn	01ch;
	};



}

void PrintObj(PDWORD pObj)
{


	CString str;
	DWORD id = *(PDWORD)(pObj);
	PWCHAR name = *(PWCHAR*)(pObj + 1);
	DWORD level = *(PDWORD)(pObj + 3);
	str.Format(L"[InjectDLL]id:%d,level:%d,name:%s", id, level, name);
	OutputDebugString(str.GetBuffer());

}

struct Person
{
	int Order;
	int ID;
	wchar_t* Name;
	char* Area;//大区
	DWORD Level;
};

void PrintObj(Person* pObj)
{


	CString str;
	DWORD id = pObj->ID;
	PWCHAR name = pObj->Name;
	DWORD level = pObj->Level;
	str.Format(L"[InjectDLL]id:%d,level:%d,name:%s", id, level, name);
	OutputDebugString(str.GetBuffer());

}


#pragma pack(push,4)
typedef struct _Tree_node {
	using _Nodeptr = _Tree_node*;
	using value_type = Person;
	_Nodeptr _Left; // left subtree, or smallest element if head
	_Nodeptr _Parent; // parent, or root of tree if head
	_Nodeptr _Right; // right subtree, or largest element if head
	char _Color; // _Red or _Black, _Black if head
	char _Isnil; // true only if head (also nil) node; TRANSITION, should be bool
	value_type _Myval; // the stored value, unused if head
}*PNode;
#pragma pack(pop)




PNode GetRootNode(PNode nPNode)
{
	PNode rNode = NULL;

	while (TRUE)
	{
		if (nPNode->_Color == 1 && nPNode->_Isnil == 1)
		{
			rNode = nPNode->_Parent;
			break;
		}

		nPNode = nPNode->_Parent;
	}
	return rNode;
}



void preOrderTraveral(PNode node) {
	if (node == NULL || (node->_Isnil == 1 && node->_Color == 1)) {
		return;
	}

	preOrderTraveral(node->_Left);
	preOrderTraveral(node->_Right);



	Person* pObj = (Person*)((DWORD)node + 0x10);//头节点
	PrintObj(pObj);
}
void ShouDongEachCore(PNode ptree)
{
	PNode pRootNode = GetRootNode(ptree);

	preOrderTraveral(pRootNode->_Left);
	preOrderTraveral(pRootNode->_Right);

}

void ShouDongEach(PVOID adress2, PVOID adress4, DWORD n_ecx)
{

	PVOID n_mm = malloc(4);
	DWORD nBeginAddr = 0;

	__asm
	{
		pushad;
		sub     esp, 04h;
		mov     esi, n_ecx;
		push    4;
		mov		ecx, n_mm;
		call    adress4;
		mov     ecx, [esi + 150h];
		mov     eax, n_mm;
		push    eax;
		call	adress2;
		add esp, 4;
		popad;
	};

	//__asm int 3;
	PNode p1 = (PNode)(*(PDWORD*)n_mm);
	//p1 = p1 + 2;
	//+14h
	//PNode pObj = (PNode)((DWORD)p1 + 0x14);//头节点
	/*

	1.
		父节点：
		_Color	1 '\x1'	char   黑
		_Isnil	1 '\x1'	char	父节点也为1


	2.父节点开始往下遍历。
	*/

	ShouDongEachCore(p1);

	//PrintObj(pObj);

	free(n_mm);
}

void EachMapData()
{
	const char* gstr_sub_2C4120 = "558BECFF????6A??51E8????????83????5DC2????";
	const char* gstr_sub_2C54E0 = "558BECE8????????8B4D??508B00FF??E8????????8B45??5DC2????";
	const char* gstr_sub_2C5520 = "558BECE8????????8B4D??50FF??E8????????8B45??5DC2????";
	const char* gstr_sub_2C26A0 = "558BECFF????E8????????34??5DC2????";
	HMODULE module = (HMODULE)GetGameWinBase();
	DWORD nModuleSize = 0;// GetModuleLen(module);
	YSignatureCode* CYSignatureCode = new YSignatureCode();
	PVOID adress1 = CYSignatureCode->search((const void*)module, nModuleSize, gstr_sub_2C26A0);
	PVOID adress2 = CYSignatureCode->search((const void*)module, nModuleSize, gstr_sub_2C54E0);
	PVOID adress3 = CYSignatureCode->search((const void*)module, nModuleSize, gstr_sub_2C5520);
	PVOID adress4 = CYSignatureCode->search((const void*)module, nModuleSize, gstr_sub_2C4120);
	PVOID adress5 = PVOID(((DWORD)module) + SUB_2C2FD0);
	PVOID adress6 = PVOID(((DWORD)module) + SUB_2C4C80);
	DWORD n_ecx = *(PDWORD)(((DWORD)module) + WorldOffset);
	n_ecx = *(PDWORD)(n_ecx + MapPageThisOffset);
	ShouDongEach(adress2, adress4, n_ecx);














	//__asm
	//{

	//	push n_ecx;
	//	push adress6;
	//	push adress5;
	//	push adress4;
	//	push adress3;
	//	push adress2;
	//	push adress1;
	//	call EachMapDataCore;
	//};

	//CString str2;
	//str2.Format(L"adress1:%p,adress2:%p,adress3:%p,adress4:%p", adress1, adress2, adress3, adress4);

	//AfxMessageBox(str2.GetBuffer());

}


void PatchCRC()
{
	string sTeZm = "558BEC568B75??578B??????????6690E8????????8B8E????????85C975??68????????8986????????FFD7EB??3BC874??68????????8BCEE8????????68????????FFD7EB??";
	HMODULE module = (HMODULE)GetGameWinBase();
	DWORD nModuleSize = 0;// GetModuleLen(module);
	YSignatureCode CYSignatureCode;
	PVOID adress = CYSignatureCode.search((const void*)module, nModuleSize, sTeZm);
	DWORD oldPro;
	VirtualProtect(adress, 0x1000, PAGE_EXECUTE_READWRITE, &oldPro);
	*((PCHAR)adress + 0x30) = 0xEB;
	VirtualProtect(adress, 0x1000, oldPro, &oldPro);

	//int a = CCommonMemory::ReadMemory<int>(0x123456);


	AfxMessageBox(L"过CRC成功");
}