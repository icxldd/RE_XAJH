#pragma once
#include "windows.h"
#include "LSearchCode.h"
#include "Stu.h"

class DataQuery
{
public:
	ULONG_PTR GetBeiBaoJiZi();
	

	ULONG_PTR GetBeiBaoAddr();
	int GetBeiBaoLen();
	ULONG_PTR GetBeiBaoObj();
	StuObjs GetBagDatas();


	StuObjs GetNpcs();
	ULONG_PTR GetNpcAdress();
	void Test();
	void DaGuai(DWORD addr);
	void GetObjData(ULONG_PTR ObjData, StuObj& obj, eStuObjType type);
	StuObj GetRoleData();
	ULONG_PTR GetRoleBase();
	void JiNengDaGuai(int id);
	void SelectGuaiWu(INT64 Id);
	StuObjs GetJiNengDatas();
	BOOL SkillIsCd(DWORD skillAddr);
	BOOL GetWuPingCDing(DWORD addr);
	void EatYao(StuObj obj);
	StuObjs GetDiMianWuPins();
	void JianDiMian(StuObj obj);
	void AutoRun(DWORD mapId, stuVecF3 mPos);
	StuObjs GetTasksData();
	void jieTask(DWORD taskId);
	void WanChengTask(DWORD taskId, StuObj npcObj);
	StuObjs GetKeJieTasks();

};

