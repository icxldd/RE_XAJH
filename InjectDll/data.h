#pragma once
#include "pch.h"

struct GenericData
{
	wchar_t* pstr;
	int iLen;
	int iMaxLen;
	int full1;
	int full2;

	GenericData(wchar_t* pString)
	{
		pstr = pString;
		iLen = wcslen(pString);
		iMaxLen = wcslen(pString)*2;
		full1 = 0;
		full2 = 0;
	}

};