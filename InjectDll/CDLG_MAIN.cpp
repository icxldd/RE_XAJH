// CDLG_MAIN.cpp: 实现文件
//

#include "pch.h"
#include "InjectDll.h"
#include "CDLG_MAIN.h"
#include "CGetLogs.h"
#include "CFunctions.h"
#include "LSearchCode.h"
#include "GlobalGame.h"
#include "ManagerCommon.h"

// CDLG_MAIN 对话框

IMPLEMENT_DYNAMIC(CDLG_MAIN, CDialogEx)

CDLG_MAIN::CDLG_MAIN(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	GlobalGame::InitGame();
}

CDLG_MAIN::~CDLG_MAIN()
{
}

void CDLG_MAIN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLG_MAIN, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON1, &CDLG_MAIN::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDLG_MAIN::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDLG_MAIN::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDLG_MAIN::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDLG_MAIN::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDLG_MAIN::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDLG_MAIN::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &CDLG_MAIN::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CDLG_MAIN::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CDLG_MAIN::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CDLG_MAIN::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CDLG_MAIN::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CDLG_MAIN::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CDLG_MAIN::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &CDLG_MAIN::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &CDLG_MAIN::OnBnClickedButton17)
END_MESSAGE_MAP()



void CDLG_MAIN::OnBnClickedButton8()
{
	PDWORD  n1 = *(PDWORD*)Jizhi;
	n1 = *(PDWORD*)(((char*)n1) + 0x20);
	n1 = *(PDWORD*)(((char*)n1) + 0x10);
	n1 = *(PDWORD*)(((char*)n1) + 0x24);
	DWORD ecx = (DWORD)n1;
	/*
	00789590                       | 56                       | push esi                                                     | 遍历对象CALL
00789591                       | 8B71 28                  | mov esi,dword ptr ds:[ecx+28]                                | 大小
00789594                       | 85F6                     | test esi,esi                                                 | ecx不是动参，动参是edi也就是第一个参数
00789596                       | 57                       | push edi                                                     |
00789597                       | 74 1F                    | je elementclient.7895B8                                      |
00789599                       | 8B7C24 0C                | mov edi,dword ptr ss:[esp+C]                                 |
0078959D                       | 33D2                     | xor edx,edx                                                  |
0078959F                       | 8BC7                     | mov eax,edi                                                  |
007895A1                       | F7F6                     | div esi                                                      |
007895A3                       | 8B41 1C                  | mov eax,dword ptr ds:[ecx+1C]                                | 地址
007895A6                       | 8B0490                   | mov eax,dword ptr ds:[eax+edx*4]                             |
007895A9                       | 85C0                     | test eax,eax                                                 |
007895AB                       | 74 0B                    | je elementclient.7895B8                                      |
007895AD                       | 3978 08                  | cmp dword ptr ds:[eax+8],edi                                 |
007895B0                       | 74 0D                    | je elementclient.7895BF                                      | 1
007895B2                       | 8B00                     | mov eax,dword ptr ds:[eax]                                   |
007895B4                       | 85C0                     | test eax,eax                                                 |
007895B6                       | 75 F5                    | jne elementclient.7895AD                                     |
007895B8                       | 5F                       | pop edi                                                      |
007895B9                       | 33C0                     | xor eax,eax                                                  |
007895BB                       | 5E                       | pop esi                                                      |
007895BC                       | C2 0800                  | ret 8                                                        |
007895BF                       | 8B4C24 10                | mov ecx,dword ptr ss:[esp+10]                                |
007895C3                       | 85C9                     | test ecx,ecx                                                 |
007895C5                       | 74 12                    | je elementclient.7895D9                                      |
007895C7                       | 8B50 04                  | mov edx,dword ptr ds:[eax+4]                                 |
007895CA                       | 398A E4000000            | cmp dword ptr ds:[edx+E4],ecx                                |
007895D0                       | 74 07                    | je elementclient.7895D9                                      |
007895D2                       | 5F                       | pop edi                                                      |
007895D3                       | 33C0                     | xor eax,eax                                                  |
007895D5                       | 5E                       | pop esi                                                      |
007895D6                       | C2 0800                  | ret 8                                                        |
007895D9                       | 8B40 04                  | mov eax,dword ptr ds:[eax+4]                                 |
	*/

	CString str2;
	/*int size = *(PDWORD)(ecx + 0x28);*/
	DWORD size = 0;
	CMemory::ReadDword(ecx + 0x28, size);
	DWORD addr = *(PDWORD)(ecx + 0x1c);
	str2.Format(L"[icxl]表大小：%d，表首地址：%x\n", size, addr);
	OutputDebugString(str2.GetBuffer());
	for (int i = 0; i < size; i++)
	{

		//[eax + edx * 4]


		DWORD p = *(DWORD*)(addr + i * 4);
		if (p == NULL)
		{
			continue;
		}
		//str2.Format(L"[icxl]%x\n", p);
		//OutputDebugString(str2.GetBuffer());
		if (IsBadReadPtr((PDWORD)(p + 4), 4) == 0)
		{
			DWORD p2 = *(PDWORD)(p + 4);
			/*		str2.Format(L"[icxl]%d\n", p2);
					OutputDebugString(str2.GetBuffer());*/
			if (p2 == NULL)
			{
				continue;
			}

			DWORD type = CMemory::ReadDword(p2 + 0xb4);
			float x = *(float*)(p2 + 840);
			float z = *(float*)(p2 + 844);
			float y = *(float*)(p2 + 848);


			PWCHAR pstr = *(PWCHAR*)(p2 + 0x30c);
			if (*pstr == 0)
			{
				continue;
			}
			str2.Format(L"[icxl]索引:%d,对象地址:%x,名字:%s,类型:%d,坐标x:%f,坐标y:%f,坐标z:%f\n", i, p2, pstr, type, x, y, z);
			OutputDebugString(str2.GetBuffer());
		}

	}

	/*

<Elements>
  <Element Offset="840" Vartype="Float" Bytesize="4" DisplayMethod="Unsigned Integer" BackgroundColor="80000005"/>x
  <Element Offset="844" Vartype="Float" Bytesize="4" DisplayMethod="Unsigned Integer" BackgroundColor="80000005"/>z
  <Element Offset="848" Vartype="Float" Bytesize="4" DisplayMethod="Unsigned Integer" BackgroundColor="80000005"/>y
</Elements>
	*/
	//0xb4	7 npc 6怪物


	// TODO: 在此添加控件通知处理程序代码
}



void CDLG_MAIN::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	//ULONG add = searchCode("xajh.exe",".text","A1????????85C074048B40??C333C0C3A1????????85C0740B8B40??85C074048B40??C333C0C3",0);
	//CString str2;
	//str2.Format(L"[icxl]%x\n", add);
	//OutputDebugString(str2.GetBuffer());



	//FindCode fs[2] = { 0 };
	//initFindCodeStruct(&fs[0], "A1????????85C0740E8B40??85C074078B80????????C333C0C3", 0, 0);
	//initFindCodeStruct(&fs[1], "A1????????85C074048B40??C333C0C3A1????????85C0740B8B40??85C074048B40??C333C0C3", 0x20, 0);
	//必须对准


	//HMODULE moduleHandle = GetGameWinBase("xajh.exe");
	//ULONG size = GetModuleLen(moduleHandle);
	//ULONG add = findAddressByCode((ULONG_PTR)moduleHandle, size + (ULONG_PTR)moduleHandle, fs, 2);

	//CString str2;
	//str2.Format(L"[icxl]%x\n", add);
	//OutputDebugString(str2.GetBuffer());
	/*GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<int>(0x123);*/
	//GlobalGame* aa = GlobalGame::GetInstance();


	//int len = GlobalGame::GetInstance()->GetDataQuery()->GetBeiBaoLen();
	//PULONG_PTR addr = (PULONG_PTR)GlobalGame::GetInstance()->GetDataQuery()->GetBeiBaoAddr();
	//DebugPrint("数组长度：%x,数组地址:%p\r\n", len, addr);


	//for (size_t i = 0; i < len; i++)
	//{
	//	PULONG_PTR adddr = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<PULONG_PTR>((ULONG_PTR)addr);
	//	if (adddr == NULL)
	//	{
	//		continue;
	//	}
	//	DebugPrint("物品数组对象[%d]：%p,物品数量:%d\r\n", i, adddr, GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>((ULONG_PTR)adddr + BeiBaoWupinLenOffset));
	//	addr++;
	//}
	//


	GlobalGame::GetInstance()->GetDataQuery()->GetBagDatas().DebugPrintInfo();
}


void CDLG_MAIN::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	GlobalGame::GetInstance()->GetDataQuery()->GetNpcs().DebugPrintInfo();
}


void CDLG_MAIN::OnBnClickedButton3()
{
	auto dizhi = GlobalGame::GetInstance()->GetDataQuery()->GetNpcs().mData[0].maddr;
	// TODO: 在此添加控件通知处理程序代码
	GlobalGame::GetInstance()->GetDataQuery()->DaGuai((DWORD)dizhi);
}


void CDLG_MAIN::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	GlobalGame::GetInstance()->GetDataQuery()->GetRoleData().DebugPrintInfo();
}


void CDLG_MAIN::OnBnClickedButton5()
{
	StuObjs jns = GlobalGame::GetInstance()->GetDataQuery()->GetJiNengDatas();
	for (size_t i = 0; i < jns.mData.size(); i++)
	{
		DWORD jdid = jns.mData[i].mId32;
		GlobalGame::GetInstance()->GetDataQuery()->JiNengDaGuai(jdid);
	}

	// TODO: 在此添加控件通知处理程序代码
}


void CDLG_MAIN::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	GlobalGame::GetInstance()->GetDataQuery()->GetJiNengDatas().DebugPrintInfo();
}


void CDLG_MAIN::OnBnClickedButton7()
{

	//GlobalGame::GetInstance()->GetKillManager()->AddFileter("伏击剑客");
	GlobalGame::GetInstance()->m_DaGuaiIsEnable = !GlobalGame::GetInstance()->m_DaGuaiIsEnable;
	DebugPrint("%d,%d", GlobalGame::GetInstance()->m_DaGuaiIsEnable, GlobalGame::GetInstance()->m_MainIsEnable);
}



void CDLG_MAIN::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	GlobalGame::GetInstance()->GetDataQuery()->Test();
}


void CDLG_MAIN::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	GlobalGame::GetInstance()->GetDataQuery()->GetDiMianWuPins().DebugPrintInfo();
}


void CDLG_MAIN::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	auto dimians = GlobalGame::GetInstance()->GetDataQuery()->GetDiMianWuPins();
	auto role = GlobalGame::GetInstance()->GetDataQuery()->GetRoleData();

	auto obj = GetUpdateNearDimian(role, dimians);
	obj.DebugPrintInfo();


	GlobalGame::GetInstance()->GetDataQuery()->JianDiMian(obj);


}


void CDLG_MAIN::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码

	GlobalGame::GetInstance()->GetDataQuery()->AutoRun(0x44, stuVecF3(18, -78, 0));
}


void CDLG_MAIN::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	GlobalGame::GetInstance()->GetDataQuery()->GetTasksData().DebugPrintInfo();
}


void CDLG_MAIN::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	GlobalGame::GetInstance()->GetDataQuery()->jieTask(0x14da);
}


void CDLG_MAIN::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	auto npcs= GlobalGame::GetInstance()->GetDataQuery()->GetNpcs();
	auto npc  = npcs.GetDataByNames({ "令狐冲" });
	GlobalGame::GetInstance()->GetDataQuery()->WanChengTask(0x14da, npc);
}


void CDLG_MAIN::OnBnClickedButton16()
{
	// TODO: 在此添加控件通知处理程序代码.
	GlobalGame::GetInstance()->GetDataQuery()->GetKeJieTasks().DebugPrintInfo();
}

static DWORD toushi_jizhi = GlobalGame::GetInstance()->CalcJiZhiContext(searchCode("xajh.exe", ".text", "E8????????83C4088D8D10FCFFFF51B9????????E8????????", 0xf));
void CDLG_MAIN::OnBnClickedButton17()
{
	// TODO: 在此添加控件通知处理程序代码

	DWORD val = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(toushi_jizhi + 0x30);
	val = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(val + 0x8c);
	val = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(val + 0x1F54);
	GlobalGame::GetInstance()->GetMemoryOp()->WriteMemory<BYTE>(val + 0x2D1,1);

}
