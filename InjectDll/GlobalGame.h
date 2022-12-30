#pragma once
#include "CCommonMemory.h"
#include "OffsetData.h"
#include "DataQuery.h"
#include "LLog.h"

#include <vector>
#include "WorkThread.h"
#include "ManagerKillMonster.h"
using namespace std;
class GlobalGame
{
private:
	static GlobalGame* m_GlobalGame;

public:
	static GlobalGame* GetInstance()
	{
		if (m_GlobalGame == nullptr) {
			m_GlobalGame = new GlobalGame();
		}

		return m_GlobalGame;
	}

	static void InitGame();

public:
	~GlobalGame() {
		free(m_CommonMemory);
		free(m_GameDataArrary);
		free(m_DataQuery);
		free(m_WorkThread);
		free(m_ManagerKillMonster);
		DebugPrint("资源释放");
	}
	GlobalGame() {
		m_CommonMemory = new CCommonMemory();
		m_GameDataArrary = new vector<ULONG_PTR>();
		m_GameDataArrary->resize(0x1000);
		m_DataQuery = new DataQuery();
		m_ManagerKillMonster = new ManagerKillMonster();
		m_DaGuaiIsEnable = FALSE;
		m_MainIsEnable = TRUE;
		m_WorkThread = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitMainWork, 0, 0, 0);//不会堵塞
		m_WorkThread2 = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DaGuai, 0, 0, 0);//不会堵塞
	}
	CCommonMemory* GetMemoryOp()
	{
		return m_CommonMemory;
	}

	vector<ULONG_PTR>* GetGameData()
	{
		return m_GameDataArrary;
	}
	ULONG_PTR* GetGameData(int index)
	{
		return &((*m_GameDataArrary)[index]);
	}
	ULONG_PTR GetGameDataValue(int index)
	{
		return ((*m_GameDataArrary)[index]);
	}
	DataQuery* GetDataQuery()
	{
		return m_DataQuery;
	}
	ManagerKillMonster* GetKillManager()
	{
		return m_ManagerKillMonster;
	}
	ULONG_PTR GetBeiBaoJizhi()
	{
		return (*GetGameData())[BeiBaoJizhi];
	}

	ULONG_PTR GetNpcJiZhi()
	{
		return (*GetGameData())[获取NPCthis基质];
	}

	ULONG_PTR GetNpcNameCall()
	{
		return (*GetGameData())[获取NPCNameCall];
	}
	ULONG_PTR GetDaGuaiCall()
	{
		return (*GetGameData())[获取gj私有代码Call];
	}
	ULONG_PTR GetDaGuaiThis()
	{
		return (*GetGameData())[获取攻击this基质];
	}

	ULONG_PTR GetWordJizhi()
	{
		return (*GetGameData())[BeiBaoJizhi];
	}

	ULONG_PTR GetSelfJizhiCall()
	{
		return (*GetGameData())[获取自己基质CALL];
	}

	//指令的下一行开始位置+取内容4字节 = call位置
	ULONG_PTR CalcCallPos(ULONG_PTR pos)//call 0x12345678   5字节的首地址
	{//006B3EAF	006B3EB3+5D 2B 60 00
		ULONG xiaHang = pos + sizeof(ULONG_PTR) + 1;
		ULONG val = GetMemoryOp()->ReadMemory<ULONG_PTR>(pos + 1);

		//DebugPrint("CalcCallPos:0x%x,0x%x,0x%x", pos,xiaHang,val);
		return xiaHang + val;
	}

	ULONG_PTR CalcJiZhiContext(ULONG_PTR pos)//00461D7D | B9 A0D65901             | mov ecx,xajh.159D6A0                                    | <<<<<<<
	{
		ULONG nei = pos + 1;
		ULONG val = GetMemoryOp()->ReadMemory<ULONG_PTR>(nei);

		return val;
	}


public:
	HANDLE m_WorkThread;
	HANDLE m_WorkThread2;
	BOOL m_DaGuaiIsEnable;
	BOOL m_MainIsEnable;
private:
	CCommonMemory* m_CommonMemory;
	DataQuery* m_DataQuery;
	vector<ULONG_PTR>* m_GameDataArrary;
	ManagerKillMonster* m_ManagerKillMonster;

public:

	DWORD GetVirualFunc(DWORD nEcx, DWORD pianyi);
};



