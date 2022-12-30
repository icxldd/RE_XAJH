#include "ManagerCommon.h"

StuObj GetUpdateNearDimian(StuObj Role, StuObjs& DiMians)
{
	StuObj maxSpaceObj = StuObj();
	for (auto& obj : DiMians.mData)
	{
		float space = obj.mZuoBiao.Dis(Role.mZuoBiao);
		obj.m_Ȩ�� = 10000 - (int)space;
		if (obj.m_Ȩ�� > maxSpaceObj.m_Ȩ��)
		{
			maxSpaceObj = obj;
		}
	}

	return maxSpaceObj;
}
