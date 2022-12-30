#include "ManagerEatYao.h"
vector<Yao> YaoList = {
	{"莲花卷",回血},
	{"返元散",回蓝}
};

StuObj GetYao(StuObjs& yaos, StuObj role)
{

	auto t = YaoList;

	if (role.m_Hp._Ptr() < 50)
	{
		for (auto nit = begin(t); nit != end(t); nit++)
		{
			if (nit->mtype == 回血)
			{
				return yaos.GetDataByNames({ nit->mName });
			}
		}
	}

	if (role.m_Mp._Ptr() < 30)
	{
		for (auto nit = begin(t); nit != end(t); nit++)
		{
			if (nit->mtype == 回蓝)
			{
				return yaos.GetDataByNames({ nit->mName });
			}
		}
	}

	return StuObj();

}
