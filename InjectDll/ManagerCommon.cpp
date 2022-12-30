#include "ManagerCommon.h"

StuObj GetUpdateNearDimian(StuObj Role, StuObjs& DiMians)
{
	StuObj maxSpaceObj = StuObj();
	for (auto& obj : DiMians.mData)
	{
		float space = obj.mZuoBiao.Dis(Role.mZuoBiao);
		obj.m_权重 = 10000 - (int)space;
		if (obj.m_权重 > maxSpaceObj.m_权重)
		{
			maxSpaceObj = obj;
		}
	}

	return maxSpaceObj;
}
