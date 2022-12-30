#include "WorkThread.h"
#include "LLog.h"
#include "windows.h"
#include "GlobalGame.h"
#include "ManagerSkill.h"
#include "ManagerEatYao.h"
void DaGuai()
{
	while (1)
	{
		if (GlobalGame::GetInstance()->m_DaGuaiIsEnable == FALSE)
		{
			Sleep(1000);
			continue;
		}
		auto Objs = GlobalGame::GetInstance()->GetDataQuery()->GetNpcs();
		auto role = GlobalGame::GetInstance()->GetDataQuery()->GetRoleData();

		GlobalGame::GetInstance()->GetKillManager()->UpdateNearMonster(role, Objs);
		auto NearMonster = Objs.GetMaxȨֵ();
		if (NearMonster.maddr == 0)
		{
			return;
		}
		GlobalGame::GetInstance()->GetDataQuery()->SelectGuaiWu(NearMonster.mId64);
		GlobalGame::GetInstance()->GetKillManager()->KillMonster(NearMonster.mId64);

		while (GlobalGame::GetInstance()->GetKillManager()->GetKillMonSter() != 0)
		{
			Sleep(1000);
		}
	}
}
void InitMainWork()
{
	for (;;)
	{
		if (GlobalGame::GetInstance()->m_MainIsEnable == FALSE)
		{
			Sleep(1000);
			continue;
		}
		Sleep(1000);
		auto role = GlobalGame::GetInstance()->GetDataQuery()->GetRoleData();
		auto bags = GlobalGame::GetInstance()->GetDataQuery()->GetBagDatas();
		auto yao = GetYao(bags, role);

		if (yao.maddr != 0)
		{
			GlobalGame::GetInstance()->GetDataQuery()->EatYao(yao);
		}

		DWORD64 id = GlobalGame::GetInstance()->GetKillManager()->GetKillMonSter();
		if (id == 0)
		{
			continue;
		}
		//GlobalGame::GetInstance()->GetDataQuery()->SelectGuaiWu(id);
		auto objs = GlobalGame::GetInstance()->GetDataQuery()->GetNpcs();
		auto obj = GlobalGame::GetInstance()->GetKillManager()->UpdateDataById(objs, id);
		if (obj.maddr == 0 || obj.isMonster() == false || obj.IsDie())
		{
			GlobalGame::GetInstance()->GetKillManager()->KillTheMonster();
			continue;
		}

		//DebugPrint("%llx", id);
		int skilId = GetSkill();

		if (skilId == 0 || role.m_Mp.min < 50 || obj.m_Hp._Ptr() < 20)
		{
			GlobalGame::GetInstance()->GetDataQuery()->DaGuai((DWORD)obj.maddr);
		}
		else
		{
			GlobalGame::GetInstance()->GetDataQuery()->JiNengDaGuai(skilId);
		}


		//


	}
}
