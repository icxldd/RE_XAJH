#pragma once
#include <string>
#include <vector>
#include "Stu.h"
using namespace std;
enum  YaoType
{
	��Ѫ,
	����
};

struct Yao
{
	string mName;
	YaoType mtype;
};

StuObj GetYao(StuObjs &yaos,StuObj role);

