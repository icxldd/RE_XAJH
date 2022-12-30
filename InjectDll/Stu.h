#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <math.h>
using namespace std;
enum eStuObjType {

	eStuObjType_���� = 0,
	eStuObjType_��Ʒ = 1,
	eStuObjType_�Լ� = 2,
	eStuObjType_���� = 3,
	eStuObjType_������Ʒ = 4,
	eStuObjType_���� = 5,

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

	//-------------------------------------����
	PULONG_PTR maddr;//��ַ
	string mName;//����
	ULONG mId32;//id
	ULONG64 mId64;//id
	int m_LV = 0;//�ȼ�

	//------------------------------------��Ʒ�ṹ
	stuVecI3 mNum;//��Ʒ��ǰ������
	int mPos;//��Ʒ�ڱ�����λ��

	//-------------------����|��Ʒ
	BOOL IsCDing;//�Ƿ�����ID


	//------------------------------------����
	stuVecF3 mZuoBiao;

	//hp
	stuVecI3 m_Hp;
	//mp
	stuVecI3 m_Mp;
	//����
	stuVecI3 m_Tl;


	//------------------------------����
	UINT m_Ȩ�� = 0;


	//------------------------------����
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
	StuObj GetMaxȨֵ();
};
