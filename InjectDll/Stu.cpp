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

StuObj StuObjs::GetMax权值()
{
	int NowValue = 0;
	StuObj nowObj;
	for (auto& obj : mData)
	{
		if (obj.m_权重 == 0)
		{
			continue;
		}
		if (obj.m_权重 > NowValue)
		{
			NowValue = obj.m_权重;
			nowObj = obj;
		}
	}
	return nowObj;
}

void StuObj::DebugPrintInfo()
{

	switch (mType)
	{

	case eStuObjType_对象: {
		DebugPrint("类型：%s,addr:%p,名字:%s,id:%llx,x:%f,y:%f,z:%f,LV:%d,HP:%d\r\n", mTypeNum == 0x16 ? "怪物" : mTypeNum == 0x17 ? "NPC" : "对象",
			maddr, mName.c_str(), mId64, mZuoBiao.x, mZuoBiao.y, mZuoBiao.z, m_LV, m_Hp._Ptr());
		break;
	}
	case eStuObjType_自己: {
		DebugPrint("类型：%s,addr:%p,名字:%s,id:%llx,x:%f,y:%f,z:%f,LV:%d,HP:%d,%d MP %d,%d\r\n", "自己",
			maddr, mName.c_str(), mId64, mZuoBiao.x, mZuoBiao.y, mZuoBiao.z, m_LV, m_Hp.min, m_Hp.max, m_Mp.min, m_Mp.max);
		break;
	}
	case eStuObjType_物品: {
		DebugPrint("类型：%s,位置:%d,addr:%p,名字:%s,id:%x,数量:%d,是否正在CD:%d\r\n", "物品", mPos, maddr, mName.c_str(), mId32, mNum.min, IsCDing);

		break;
	}
	case eStuObjType_地面物品: {
		DebugPrint("类型：%s,addr:%p,名字:%s,id:%llx,x:%f,y:%f,z:%f\r\n", "地面物品",  maddr, mName.c_str(), mId64, mZuoBiao.x, mZuoBiao.y, mZuoBiao.z);

		break;
	}
	case eStuObjType_技能: {
		DebugPrint("id:%x,名称:%s,地址:%x,是否正在CD:%d", mId32, mName.c_str(), maddr, IsCDing);

		break;
	}

	case eStuObjType_任务: {
		DebugPrint("id:%x,名称:%s,地址:%x,已完成数量:%d", mId32, mName.c_str(), maddr,mKillNum);

		break;
	}


	default:
		break;
	}

}
