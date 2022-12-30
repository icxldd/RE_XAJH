#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <math.h>
using namespace std;
enum eStuObjType {

	eStuObjType_对象 = 0,
	eStuObjType_物品 = 1,
	eStuObjType_自己 = 2,
	eStuObjType_技能 = 3,
	eStuObjType_地面物品 = 4,
	eStuObjType_任务 = 5,

};
struct stuVecI3 {

	int min = 0;
	int max = 0;
	int ptr = -1;
	stuVecI3() {}
	stuVecI3(int rmin, int rmax) {
		SetVal(rmin, rmax);
	}
	void SetVal(int rmin, int rmax)
	{
		min = rmin;
		max = rmax;
	}
	int _Ptr()
	{
		if (ptr != -1)
		{
			return ptr;
		}
		if (max == 0)
		{
			return 0;
		}
		return (int)((float)min / (float)max * 100);
	}

};

struct stuVecF3 {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	stuVecF3() {}
	stuVecF3(float x, float y, float z) :x(x), y(y), z(z) {

	}
	float Dis(stuVecF3 Obj)
	{
		float rx = x - Obj.x;

		float ry = y - Obj.y;
		float r = rx * rx + ry * ry;

		return sqrt(r);
	}

};

struct StuObj
{

public:
	eStuObjType mType;
	int mTypeNum = 0;

	//-------------------------------------公有
	PULONG_PTR maddr;//地址
	string mName;//名称
	ULONG mId32;//id
	ULONG64 mId64;//id
	int m_LV = 0;//等级

	//------------------------------------物品结构
	stuVecI3 mNum;//物品当前的数量
	int mPos;//物品在背包的位置

	//-------------------技能|物品
	BOOL IsCDing;//是否正在ID


	//------------------------------------对象
	stuVecF3 mZuoBiao;

	//hp
	stuVecI3 m_Hp;
	//mp
	stuVecI3 m_Mp;
	//体力
	stuVecI3 m_Tl;


	//------------------------------怪物
	UINT m_权重 = 0;


	//------------------------------任务
	WORD mKillNum = 0;

	void DebugPrintInfo();

	bool isMonster()
	{
		if (mTypeNum == 0x16)
		{
			return true;
		}
		return false;
	}
	bool IsDie()
	{
		return m_Hp._Ptr() == 0;
	}
};
struct StuObjs {

public:
	vector<StuObj> mData;


	StuObj GetDataByNames(vector<string> Names);

	StuObjs GetDatasByNames(vector<string> Names);

	void DebugPrintInfo();
	StuObj GetMax权值();
};
