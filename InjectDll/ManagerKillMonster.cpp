#include "ManagerKillMonster.h"

ManagerKillMonster::ManagerKillMonster()
{
	GuaiWuFilter = new vector<string>();
}

ManagerKillMonster::~ManagerKillMonster()
{
}

void ManagerKillMonster::KillMonster(DWORD64 id)
{
	m_KillMonsterID = id;
}

BOOL ManagerKillMonster::IsDie()
{
	return m_KillMonsterID == 0;
}

DWORD64 ManagerKillMonster::GetKillMonSter()
{
	return m_KillMonsterID;
}

void ManagerKillMonster::KillTheMonster()
{
	m_KillMonsterID = 0;
}

void ManagerKillMonster::UpdateNearMonster(StuObj Role, StuObjs& Monster)
{

	for (auto& obj : Monster.mData)
	{
		if (obj.isMonster())
		{
			if (obj.IsDie() || !ContainerFilter(obj.mName))
			{
				continue;
			}
			float space = obj.mZuoBiao.Dis(Role.mZuoBiao);
			obj.m_Ȩ�� = 10000 - (int)space;

			//obj.m_Ȩ�� = obj.m_Hp._Ptr() * 10000 + (1000 - space);
		}
	}

}

StuObj ManagerKillMonster::UpdateDataById(StuObjs& Obj, DWORD64 id)
{
	for (auto& obj : Obj.mData)
	{
		if (obj.mId64 == id)
		{
			return obj;
		}
	}
	return StuObj();
}


void ManagerKillMonster::AddFileter(string sName)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	GuaiWuFilter->push_back(sName);
}


void ManagerKillMonster::ClearFileter()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	GuaiWuFilter->clear();
}


BOOL ManagerKillMonster::ContainerFilter(string sName)
{
	if (GuaiWuFilter->size()==0)
	{
		return TRUE;
	}
	for (auto nit = begin(*GuaiWuFilter); nit != end(*GuaiWuFilter); nit++)
	{
		if (*nit == sName)
		{
			return TRUE;
		}
	}

	// TODO: �ڴ˴����ʵ�ִ���.
	return FALSE;
}
