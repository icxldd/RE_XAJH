#include "Stu.h"
#include "LLog.h"

StuObj StuObjs::GetDataByNames(vector<string> Names)
{
	for (auto it = begin(mData); it != end(mData); it++)
	{
		for (auto nit = begin(Names); nit != end(Names); nit++)
		{
			if (*nit == it->mName)
			{
				return *it;
			}
		}
	}
	return StuObj();
}

StuObjs StuObjs::GetDatasByNames(vector<string> Names)
{
	StuObjs Value;

	for (auto it = begin(mData); it != end(mData); it++)
	{
		for (auto nit = begin(Names); nit != end(Names); nit++)
		{
			if (*nit == it->mName)
			{
				Value.mData.push_back(*it);
				break;
			}
		}
	}
	return Value;

}

void StuObjs::DebugPrintInfo()
{
	for (auto it = begin(mData); it != end(mData); it++) {
		it->DebugPrintInfo();
	}

}

StuObj StuObjs::GetMaxȨֵ()
{
	int NowValue = 0;
	StuObj nowObj;
	for (auto& obj : mData)
	{
		if (obj.m_Ȩ�� == 0)
		{
			continue;
		}
		if (obj.m_Ȩ�� > NowValue)
		{
			NowValue = obj.m_Ȩ��;
			nowObj = obj;
		}
	}
	return nowObj;
}

void StuObj::DebugPrintInfo()
{

	switch (mType)
	{

	case eStuObjType_����: {
		DebugPrint("���ͣ�%s,addr:%p,����:%s,id:%llx,x:%f,y:%f,z:%f,LV:%d,HP:%d\r\n", mTypeNum == 0x16 ? "����" : mTypeNum == 0x17 ? "NPC" : "����",
			maddr, mName.c_str(), mId64, mZuoBiao.x, mZuoBiao.y, mZuoBiao.z, m_LV, m_Hp._Ptr());
		break;
	}
	case eStuObjType_�Լ�: {
		DebugPrint("���ͣ�%s,addr:%p,����:%s,id:%llx,x:%f,y:%f,z:%f,LV:%d,HP:%d,%d MP %d,%d\r\n", "�Լ�",
			maddr, mName.c_str(), mId64, mZuoBiao.x, mZuoBiao.y, mZuoBiao.z, m_LV, m_Hp.min, m_Hp.max, m_Mp.min, m_Mp.max);
		break;
	}
	case eStuObjType_��Ʒ: {
		DebugPrint("���ͣ�%s,λ��:%d,addr:%p,����:%s,id:%x,����:%d,�Ƿ�����CD:%d\r\n", "��Ʒ", mPos, maddr, mName.c_str(), mId32, mNum.min, IsCDing);

		break;
	}
	case eStuObjType_������Ʒ: {
		DebugPrint("���ͣ�%s,addr:%p,����:%s,id:%llx,x:%f,y:%f,z:%f\r\n", "������Ʒ",  maddr, mName.c_str(), mId64, mZuoBiao.x, mZuoBiao.y, mZuoBiao.z);

		break;
	}
	case eStuObjType_����: {
		DebugPrint("id:%x,����:%s,��ַ:%x,�Ƿ�����CD:%d", mId32, mName.c_str(), maddr, IsCDing);

		break;
	}

	case eStuObjType_����: {
		DebugPrint("id:%x,����:%s,��ַ:%x,���������:%d", mId32, mName.c_str(), maddr,mKillNum);

		break;
	}


	default:
		break;
	}

}
