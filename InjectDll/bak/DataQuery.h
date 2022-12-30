#pragma once
#include "windows.h"
#include "LSearchCode.h"
#include "Stu.h"

class DataQuery
{
public:
	ULONG_PTR GetBeiBaoJiZi();
	

	ULONG_PTR GetBeiBaoAddr();
	int GetBeiBaoLen();
	ULONG_PTR GetBeiBaoObj();
	StuObjs GetBagDatas();


	StuObjs GetNpcs();
	ULONG_PTR GetNpcAdress();
	void Test();
};

