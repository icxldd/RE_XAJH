#include "ManagerEatYao.h"
vector<Yao> YaoList = {
	{"������",��Ѫ},
	{"��Ԫɢ",����}
};

StuObj GetYao(StuObjs& yaos, StuObj role)
{

	auto t = YaoList;

	if (role.m_Hp._Ptr() < 50)
	{
		for (auto nit = begin(t); nit != end(t); nit++)
		{
			if (nit->mtype == ��Ѫ)
			{
				return yaos.GetDataByNames({ nit->mName });
			}
		}
	}

	if (role.m_Mp._Ptr() < 30)
	{
		for (auto nit = begin(t); nit != end(t); nit++)
		{
			if (nit->mtype == ����)
			{
				return yaos.GetDataByNames({ nit->mName });
			}
		}
	}

	return StuObj();

}
