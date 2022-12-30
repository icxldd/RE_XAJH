#include "ManagerSkill.h"
#include "GlobalGame.h"

int GetSkill()
{

	auto skils = GlobalGame::GetInstance()->GetDataQuery()->GetJiNengDatas();

	
	for (auto nit = begin(skils.mData); nit != end(skils.mData); nit++)
	{
		if (nit->IsCDing)
		{
			continue;
		}
		else 
		{
			return nit->mId32;
		}
	}

	return 0;

}
