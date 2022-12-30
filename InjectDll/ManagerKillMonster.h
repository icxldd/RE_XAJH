#pragma once
#include <Windows.h>
#include "Stu.h"
class ManagerKillMonster
{
public:
	ManagerKillMonster();
	~ManagerKillMonster();

public:
	void KillMonster(DWORD64 id);

	BOOL IsDie();

public:
	DWORD64 GetKillMonSter();

	void KillTheMonster();

public:
	void UpdateNearMonster(StuObj Role, StuObjs& Monster);

	StuObj UpdateDataById(StuObjs& Obj, DWORD64 id);

public:
	DWORD64 m_KillMonsterID = 0;
	vector<string> *GuaiWuFilter;

	void AddFileter(string sName);
	void ClearFileter();
	BOOL ContainerFilter(string sName);
};

