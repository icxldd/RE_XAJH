#include "DataQuery.h"
#include "GlobalGame.h"
#include "OffsetData.h"
#include "Utils.h"
ULONG_PTR DataQuery::GetBeiBaoJiZi()
{
	FindCode fs[2] = { 0 };
	initFindCodeStruct(&fs[0], "A1????????85C0740E8B40??85C074078B80????????C333C0C3", 0, 0);
	initFindCodeStruct(&fs[1], "A1????????85C074048B40??C333C0C3A1????????85C0740B8B40??85C074048B40??C333C0C3", 0x20, 0);
	HMODULE moduleHandle = GetGameWinBase("xajh.exe");
	ULONG size = GetModuleLen(moduleHandle);
	ULONG add = findAddressByCode((ULONG_PTR)moduleHandle, size + (ULONG_PTR)moduleHandle, fs, 2);

	return GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>((add + 1));
}


ULONG_PTR DataQuery::GetBeiBaoObj()
{
	ULONG_PTR  jizhi = GlobalGame::GetInstance()->GetBeiBaoJizhi();
	ULONG_PTR o1 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(jizhi);
	ULONG_PTR o2 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o1 + BeiBaoOffset1);
	ULONG_PTR o3 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o2 + BeiBaoOffset2);
	ULONG_PTR o4 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o3 + BeiBaoOffset3);
	ULONG_PTR o5 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o4 + BeiBaoOffset4);
	ULONG_PTR o6 = o5 + BeiBaoOffset5;
	//DebugPrint("o1:%x,o2:%x,o3:%x,o4:%x,o5:%x,o6:%x\r\n", o1, o2, o3, o4, o5, o6);
	return o6;
}


ULONG_PTR DataQuery::GetBeiBaoAddr()
{

	ULONG_PTR o6 = GetBeiBaoObj();

	ULONG_PTR addr = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o6 + BeiBaoArrayAddrOffset);

	return addr;
}


int DataQuery::GetBeiBaoLen()
{

	ULONG_PTR o6 = GetBeiBaoObj();

	ULONG_PTR len = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o6 + BeiBaoArrayLenOffset);

	return len;
}

DWORD GetBagDataAddr(DWORD ID)
{

	DWORD Ret;
	DWORD Adr;
	GlobalGame* instance = GlobalGame::GetInstance();

	ULONG_PTR callAddr = *(instance->GetGameData(获取背包物品数据结构CALL));
	ULONG_PTR callAddr2 = *(instance->GetGameData(获取背包物品数据结构CALLthis基址));
	ULONG_PTR nEax = instance->GetMemoryOp()->ReadMemory<ULONG_PTR>(instance->GetMemoryOp()->ReadMemory<ULONG_PTR>(callAddr2) + 0x1d8);
	__asm {
		lea eax, Ret;
		push eax;
		push 0;
		push ID;
		mov ecx, nEax;
		call callAddr;
		mov Adr, eax;
	};



	return Adr;
}

StuObjs DataQuery::GetBagDatas()
{

	int len = GlobalGame::GetInstance()->GetDataQuery()->GetBeiBaoLen();
	PULONG_PTR addr = (PULONG_PTR)GlobalGame::GetInstance()->GetDataQuery()->GetBeiBaoAddr();
	//DebugPrint("数组长度：%x,数组地址:%p\r\n", len, addr);


	StuObjs BagData;


	for (size_t i = 0; i < len; i++)
	{
		ULONG_PTR adddr = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(addr);
		if (adddr == NULL)
		{
			continue;
		}
		StuObj obj;
		obj.mType = eStuObjType_物品;
		obj.maddr = (PULONG_PTR)adddr;
		obj.mPos = i;
		obj.mId32 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG>(adddr + BeiBaoWupinIDOffset);
		obj.mNum.min = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG>(adddr + BeiBaoWupinLenOffset);

		DWORD DataAddr = GetBagDataAddr(obj.mId32);
		if (DataAddr)
		{
			wstring str((wchar_t*)(DataAddr + CallBagDataNameOffset));
			obj.mName = Utils::WstringToAnsi(str);
		}
		obj.IsCDing = GetWuPingCDing((DWORD)obj.maddr);
		BagData.mData.push_back(obj);
		//DebugPrint("物品数组对象[%d]：%p,物品数量:%d\r\n", i, adddr, GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>((ULONG_PTR)adddr + BeiBaoWupinLenOffset));
		addr++;
	}


	return BagData;
}

string GetNpcName(ULONG_PTR thisEcx)
{
	ULONG_PTR addr = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(thisEcx);
	ULONG_PTR adress = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(addr + 0x88);
	//ULONG_PTR adress = GlobalGame::GetInstance()->GetNpcNameCall();
	ULONG_PTR dret = 0;
	__asm {
		mov ecx, thisEcx;
		call adress;
		mov dret, eax;
	};
	//DebugPrint("名字：%s", dret);
	string rString = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemoryUTA((wchar_t*)(dret));
	return rString;
}
StuObjs DataQuery::GetNpcs()
{
	ULONG_PTR BASE = GetNpcAdress();
	ULONG_PTR adress = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(BASE + NPCAdressOffset);
	ULONG_PTR size = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(BASE + NPCSizeOffset);
	//DebugPrint("%p,%p", adress, size);
	StuObjs objs;
	for (size_t i = 0; i < size; i++)
	{

		ULONG_PTR ObjPoint = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(adress + i * 4);

		if (ObjPoint == 0)
		{
			continue;
		}
		//DebugPrint("外层对象地址%p\r\n", ObjPoint);
		for (; ObjPoint != 0;)
		{
			StuObj obj;
			ULONG_PTR ObjData = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(ObjPoint + 0x4);

			GetObjData(ObjData, obj, eStuObjType_对象);
			objs.mData.push_back(obj);
			ObjPoint = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(ObjPoint + 0x0);
		}


	}
	return objs;
}

void DataQuery::GetObjData(ULONG_PTR ObjData, StuObj& obj, eStuObjType type)
{
	obj.mType = type;
	obj.maddr = (PULONG_PTR)ObjData;
	obj.mZuoBiao.x = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(ObjData + NPCWeizhiOffset + 0);//x坐标加256
	obj.mZuoBiao.z = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(ObjData + NPCWeizhiOffset + 4);
	obj.mZuoBiao.y = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(ObjData + NPCWeizhiOffset + 8);
	obj.mId64 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<INT64>(ObjData + NPCIdOffset);
	//DebugPrint("---------对象地址%p,ID:%llx\r\n", ObjData, id);
	obj.mName = GetNpcName(ObjData);
	obj.mTypeNum = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<int>(ObjData + ObjTypeOffset);

	if (obj.isMonster())
	{
		int p = 0;
		p = (int)(GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(ObjData + 0x54e) * 100.0f);
		obj.m_Hp.ptr = p;
	}
	else if (obj.mType == eStuObjType_自己)
	{
		//自己
		obj.m_LV = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<int>(ObjData + 0xA6C);
		obj.m_Hp.SetVal((int)GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(ObjData + 0xA9F), (int)GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(ObjData + 0xAF7));
		obj.m_Mp.SetVal((int)GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(ObjData + 0xAA3), (int)GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(ObjData + 0xAFB));
	}




}


ULONG_PTR DataQuery::GetNpcAdress()
{
	ULONG_PTR  jizhi = GlobalGame::GetInstance()->GetNpcJiZhi();
	ULONG_PTR o1 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(jizhi);
	ULONG_PTR o2 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o1 + NPCOffset1);
	ULONG_PTR o3 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o2 + NPCOffset2);
	ULONG_PTR o5 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o3 + NPCOffset3 + NPCOffset4);
	ULONG_PTR o6 = o5 + NPCOffset5;
	return o6;
}




void DataQuery::DaGuai(DWORD addr)
{
	//[[[[[0x01597F50]+0x30]+0xc]+0x4]+0x40]
	ULONG_PTR  jizhi = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(GlobalGame::GetInstance()->GetWordJizhi());
	ULONG_PTR o1 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(jizhi + 0x30);
	ULONG_PTR o2 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o1 + 0xc);
	ULONG_PTR o3 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o2 + 0x4);
	DWORD dd = addr;
	GlobalGame::GetInstance()->GetMemoryOp()->WriteMemory<ULONG_PTR>(o3 + 0x40, dd);
	ULONG_PTR o4 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o3 + 0x40);
	//68432EF8
	//DebugPrint("%p", o4);

	//ecx	[[[[[015995E0+0x30]+0x94]+0x4]+0x8C]+0x1F54]
	DWORD n_ecx;
	jizhi = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(GlobalGame::GetInstance()->GetDaGuaiThis() + 0x30);
	o1 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(jizhi + 0x94);
	o2 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o1 + 0x4);
	o3 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o2 + 0x8c);
	o4 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o3 + 0x1F54);

	//DebugPrint("%p,%p,%p,%p,%p", jizhi, o1, o2, o3, o4);
	//------
	ULONG_PTR addrCall = GlobalGame::GetInstance()->GetDaGuaiCall();
	__asm {

		push dd;
		push 0;
		push 0;
		push 0;
		push 7;
		push 0xa;
		mov ecx, o4;
		mov eax, addrCall;
		call eax;
	};
}



void DataQuery::Test()
{

	//auto dd = GetJiNengDatas().mData;
	//for (auto nit = begin(dd); nit != end(dd); nit++)
	//{
	//	BOOL val = SkillIsCd((DWORD)nit->maddr);
	//	DebugPrint("%d", val);
	//}


	/*auto t = GlobalGame::GetInstance()->GetDataQuery()->GetBagDatas().mData;

	for (auto nit = begin(t); nit != end(t); nit++)
	{
		if (nit->mPos == 0)
		{
			GlobalGame::GetInstance()->GetDataQuery()->EatYao(*nit);
		}
	}*/
}


ULONG_PTR DataQuery::GetRoleBase()
{
	ULONG_PTR add = GlobalGame::GetInstance()->GetSelfJizhiCall();
	UINT val;
	__asm {

		mov eax, add;
		call eax;
		mov val, eax;
	};

	return val;
}


StuObj DataQuery::GetRoleData()
{
	StuObj role;
	GetObjData(GetRoleBase(), role, eStuObjType_自己);
	return role;
}

void DataQuery::JiNengDaGuai(int id)
{
	//StuObjs objs = GetNpcs();
	//SelectGuaiWu(objs.mData[0].mId64);
	ULONG_PTR jiNengCall = GlobalGame::GetInstance()->GetGameDataValue(获取技能CALL);
	ULONG_PTR selfThisCall = GlobalGame::GetInstance()->GetGameDataValue(获取人物基质ThisCall);
	ULONG_PTR  selfThis;
	__asm
	{
		mov eax, selfThisCall
		call eax;
		mov selfThis, eax;
	}
	ULONG_PTR jinengThis = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(selfThis + 0x1f58);

	DWORD pData[4] = { 0x1,0x1,0x010100ff,0x1 };
	_asm
	{
		push 1;
		push 0;
		push 1;
		push 0;
		push 0;
		push 0;
		push 0;
		lea eax, pData;
		push eax;
		push 1;
		push id;
		push 1;
		mov ecx, jinengThis;
		mov eax, jiNengCall;
		call eax;
	};
}


void DataQuery::SelectGuaiWu(INT64 Id)
{
	int height32 = (Id & 0xffffffff00000000) >> 32;
	int low32 = Id & 0x00000000ffffffff;
	ULONG_PTR call = GlobalGame::GetInstance()->GetGameDataValue(获取选中怪物CALL);

	//DebugPrint("%p,%x,%x,%llx", call, height32, low32, Id);
	__asm {
		push height32;
		push low32;
		mov eax, call;
		call eax;
		add esp, 0x8
	};
}

//获取武学分类ids
vector<int> GetJNFLIDs()
{
	int* paddr;
	vector<int> itemWuXueIds;
	ULONG_PTR selfCall = GlobalGame::GetInstance()->GetGameDataValue(获取自己基质CALL);
	//
	ULONG_PTR jiDizhi = GlobalGame::GetInstance()->GetGameDataValue(获取GetJNFLIDs);
	ULONG_PTR jiDizhiCall = GlobalGame::GetInstance()->GetGameDataValue(获取GetJNFLIDCall);

	__asm
	{
		mov eax, jiDizhi;
		mov eax, [eax];
		mov ecx, [eax + 0x260];


		call selfCall;
		mov eax, [eax + 0xa70];
		push eax;

		mov eax, jiDizhiCall;
		call eax;
		lea edi, [eax + 0x620];
		mov paddr, edi;
	};
	for (size_t i = 0; i < MaxWuXue; i++)
	{
		if (!(*paddr))
		{
			paddr++;
			continue;
		}
		itemWuXueIds.push_back(*paddr);
		paddr++;
	}

	return itemWuXueIds;
}


//根据武学分类ID获取武学结构
DWORD GetJNFLDetail(int id)
{
	ULONG_PTR jiDizhi = GlobalGame::GetInstance()->GetGameDataValue(获取GetJNFLDetail1);
	ULONG_PTR jiDizhiCall = GlobalGame::GetInstance()->GetGameDataValue(获取GetJNFLDetailCall);
	ULONG_PTR jiDizhiCall2 = GlobalGame::GetInstance()->GetGameDataValue(获取GetJNFLDetailCall2);
	int item = 0;
	DWORD rVal;
	__asm
	{
		mov eax, jiDizhi;
		mov ecx, [eax];
		mov eax, jiDizhiCall;
		call eax;

		mov ecx, eax;

		lea eax, item;
		push eax;
		push 0;
		push id;
		mov eax, jiDizhiCall2;
		call eax;
		mov rVal, eax;
	};

	return rVal;

}

DWORD GetJNDetail(int id)
{
	ULONG_PTR jiDizhiCall = GlobalGame::GetInstance()->GetGameDataValue(获取GetJNDetailCall1);
	ULONG_PTR jiDizhiCall2 = GlobalGame::GetInstance()->GetGameDataValue(获取GetJNDetailCall2);
	__asm
	{
		mov eax, jiDizhiCall;
		call eax;
		mov edx, [eax + 0x28];
		mov ecx, edx;

		push id;
		mov eax, jiDizhiCall2;
		call eax;

	};
}


BOOL FindVectorObj(vector<INT64> v, INT64 val)
{
	BOOL rVal = FALSE;
	for (auto nit = begin(v); nit != end(v); nit++)
	{
		if (*nit == val)
		{
			rVal = TRUE;
		}
	}
	return rVal;
}

vector<INT64> GetAllJNIDs()
{
	vector<INT64> rVal;
	vector<int> itemWuXueIds = GetJNFLIDs();;

	for (size_t j = 0; j < itemWuXueIds.size(); j++)
	{
		//根据武学分类ID获取武学结构
		DWORD addr = GetJNFLDetail(itemWuXueIds[j]);

		int fval = *(PDWORD)(addr + 0x60);
		if (fval == 1)
		{
			INT32* ids = (INT32*)(addr + 0x164);
			for (size_t i = 0; i < 0x20; i++)
			{
				if (*ids)
				{
					//DebugPrint("i:%d,%p,%x", i, ids, *ids);
					INT64 itemval = *ids;

					if (!FindVectorObj(rVal, itemval))
					{
						rVal.push_back(itemval);
					}
				}
				ids++;
			}
		}
		else if (fval == 2)
		{
			INT64* ids = (INT64*)(addr + 0x164);
			for (size_t i = 0; i < 0x20; i++)
			{
				if (*ids)
				{
					//DebugPrint("i:%d,%p,%llx", i, ids, *ids);

					if (!FindVectorObj(rVal, *ids))
					{
						rVal.push_back(*ids);
					}
				}

				ids++;
			}
		}
	}
	return rVal;
}


StuObjs DataQuery::GetJiNengDatas()
{

	StuObjs objs;

	vector<INT64> jnids = GetAllJNIDs();

	for (size_t i = 0; i < jnids.size(); i++)
	{
		//DebugPrint("%llx", jnids[i]);
		int id = static_cast<int>(jnids[i]);

		//DebugPrint("i:%d,%x", i, jnids[i]);



		DWORD  jnAddr = GetJNDetail(id);
		if (jnAddr)
		{
			DWORD nameAddr = *(PDWORD)(jnAddr + 0x24);
			string rString = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemoryUTA((wchar_t*)(nameAddr));
			StuObj obj;
			obj.mType = eStuObjType_技能;
			obj.maddr = (PULONG)jnAddr;
			obj.mId32 = id;
			obj.mName = rString;
			obj.IsCDing = SkillIsCd((DWORD)jnAddr) == 0;
			objs.mData.push_back(obj);
			/*DebugPrint("id:%x,名称:%s,%x", id, rString.c_str(), nameAddr);*/
		}

	}

	return objs;
}


BOOL DataQuery::SkillIsCd(DWORD skillAddr)
{
	ULONG_PTR Call1 = GlobalGame::GetInstance()->GetGameDataValue(获取人物偏移90Call);
	ULONG_PTR Call2 = GlobalGame::GetInstance()->GetGameDataValue(获取技能信息Call);
	ULONG_PTR Call3 = GlobalGame::GetInstance()->GetGameDataValue(根据技能信息获取冷却Call);
	int result = 0;
	DWORD n4C7190Ecx = 0;
	DWORD n6BEE30Ecx = 0;
	ULONG_PTR zhizhen = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(skillAddr + 0x34);
	DWORD jinengFlag = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(zhizhen);

	__asm
	{
		pushad;
		mov eax, Call1;
		call eax;
		mov esi, [eax + 0x24];
		mov n4C7190Ecx, esi;


		push jinengFlag;
		mov ecx, n4C7190Ecx;
		mov eax, Call2;
		call eax;
		mov n6BEE30Ecx, eax;


		mov ecx, n6BEE30Ecx;
		mov eax, Call3;
		call eax;
		mov result, eax;

		popad;
	};

	return result;
}


BOOL DataQuery::GetWuPingCDing(DWORD addr)
{
	DWORD v1 = 0;
	DWORD v2 = 0;
	DWORD add = GlobalGame::GetInstance()->GetVirualFunc(addr, 0x2c);
	DWORD ret;
	__asm
	{


		lea eax, v1;
		push eax;
		lea eax, v2;
		push eax;

		mov ecx, addr;
		call add;
		mov ret, eax;
	};

	return ret == 0 ? FALSE : TRUE;
}


void DataQuery::EatYao(StuObj obj)
{
	ULONG_PTR call1 = GlobalGame::GetInstance()->GetGameDataValue(获取吃药call);
	int pos = obj.mPos;
	__asm
	{
		mov ecx, pos;
		push ecx;
		push 2;
		mov eax, call1;
		call eax;
		add esp, 8;
	};
}

void GetDiMianItemData(DWORD itemAddr, StuObj& obj)
{
	obj.mType = eStuObjType_地面物品;
	obj.mId64 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<INT64>(itemAddr + 0x178);

	DWORD pianyi = 0x190;
	obj.mZuoBiao.x = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(itemAddr + pianyi + 0);
	obj.mZuoBiao.z = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(itemAddr + pianyi + 4);
	obj.mZuoBiao.y = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(itemAddr + pianyi + 8);
	obj.maddr = (PULONG)itemAddr;

	DWORD nameAddr = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(itemAddr + 0x55c);
	obj.mName = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemoryUTA((wchar_t*)(nameAddr));
}
StuObjs DataQuery::GetDiMianWuPins()
{
	StuObjs objs;
	DWORD addr = 0;
	DWORD itemResult;
	//获取自己基质CALL
	ULONG_PTR call1 = GlobalGame::GetInstance()->GetGameDataValue(获取自己基质CALL);
	__asm {

		pushad;

		call call1;
		mov itemResult, eax;
		popad;
	};
	DWORD necx = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(itemResult + 0x1f54);
	DWORD necx2 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(necx + 8);
	DWORD call2 = GlobalGame::GetInstance()->GetVirualFunc(necx2, 0x8c);
	__asm
	{
		//mov eax, 0x007568F0;
		mov eax, call2;
		call eax;
		mov edi, [eax + 0x78];

		lea eax, [edi + 0x14];
		mov addr, eax;
	};
	if (addr == 0)
	{
		return StuObjs();
	}
	int maxCount = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<int>(addr + 0x10);
	DWORD startAddr = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(addr + 0x8);
	DebugPrint("%p,%p", addr, startAddr);
	for (size_t i = 0; i < maxCount; i++)
	{
		DWORD wupinObj = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(startAddr + i * 4);
		if (wupinObj == 0)
		{
			continue;
		}

		do
		{
			DWORD ItemAddr = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(wupinObj + 4);


			StuObj item;
			GetDiMianItemData(ItemAddr, item);
			objs.mData.push_back(item);

			//链表
			wupinObj = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(wupinObj + 0);

		} while (wupinObj);

	}


	// TODO: 在此处添加实现代码.
	return objs;
}


void DataQuery::JianDiMian(StuObj obj)
{
	// TODO: 在此处添加实现代码.
	ULONG_PTR call11 = GlobalGame::GetInstance()->GetGameDataValue(获取捡物品基质call);
	ULONG_PTR call12 = GlobalGame::GetInstance()->GetGameDataValue(获取捡物品call);

	DWORD call2 = GlobalGame::GetInstance()->GetVirualFunc((DWORD)obj.maddr, 0x84);
	DWORD necx = (DWORD)obj.maddr;
	DWORD jId = 0;
	int height32 = (obj.mId64 & 0xffffffff00000000) >> 32;
	int low32 = obj.mId64 & 0x00000000ffffffff;
	__asm
	{
		mov ecx, necx;
		call call2;
		mov jId, eax;
	};


	__asm
	{
		/*mov eax, 0x00455FA0;*/
		mov eax, call11;
		call eax;
		mov ecx, eax;


		push jId;
		push height32;
		push low32;
		/*mov eax, 0x0068F6A0;*/
		mov eax, call12;
		call eax;
	};


}


void DataQuery::AutoRun(DWORD mapId, stuVecF3 mPos)
{
	// TODO: 在此处添加实现代码.
	ULONG_PTR call11 = GlobalGame::GetInstance()->GetGameDataValue(获取自动寻路基质call);
	ULONG_PTR call12 = GlobalGame::GetInstance()->GetGameDataValue(获取自动寻路call);


	float item[3] = { mPos.x,mPos.z,mPos.y };

	__asm
	{
		mov eax, call11;
		call eax;
		mov ecx, [eax + 0x2B4];

		push 0;
		push 0;
		push 0;
		push 0;
		lea eax, item;
		push  eax;
		/*push 44;*/
		push mapId;
		push 0;
		push 0;
		push 0;
		push 0;
		mov eax, call12;
		call eax;
	};


}

static DWORD GetTaskName_c1 = GlobalGame::GetInstance()->CalcCallPos(searchCode("xajh.exe", ".text", "8B58??57895C2434E8????????8BC8E8????????8BE88BCB896C2434E8????????8A1032C984D27616", 8));
static DWORD GetTaskName_c2 = GlobalGame::GetInstance()->CalcCallPos(searchCode("xajh.exe", ".text", "8B58??57895C2434E8????????8BC8E8????????8BE88BCB896C2434E8????????8A1032C984D27616", 0xf));
string GetTaskName(DWORD id)
{
	DWORD nptr = 0;
	__asm
	{
		mov eax, GetTaskName_c1;
		call eax;
		mov ecx, eax;
		mov eax, GetTaskName_c2;
		push id;
		call eax;
		mov nptr, eax;
	};

	return GlobalGame::GetInstance()->GetMemoryOp()->ReadMemoryUTA((wchar_t*)(nptr + 8));
}


static DWORD GetTasksData_c = searchCode("xajh.exe", ".text", "A1????????8945CC8B48??3BCB741168????????8B15????????", 0);
static DWORD GetTasksData_call = GlobalGame::GetInstance()->CalcCallPos(searchCode("xajh.exe", ".text", "897C2418E8????????8B4C242489442410E8????????", 0x11));
StuObjs DataQuery::GetTasksData()
{

	/*00C540D6 | A1 10C05901 | mov eax, dword ptr ds : [0x159C010] |
		00C540DB | 8B48 30 | mov ecx, dword ptr ds : [eax + 0x30] |
		00C540DE | 8B51 08 | mov edx, dword ptr ds : [ecx + 0x8] |
		00C540E1 | 8B8A C4030000 | mov ecx, dword ptr ds : [edx + 0x3C4] |*/
	StuObjs taskList;
	DWORD c = GetTasksData_c;
	DWORD jizhi = GlobalGame::GetInstance()->CalcJiZhiContext(c);
	DWORD val = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(jizhi);
	val = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(val + 0x90);
	val = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(val + 0x30);
	DWORD nEcx = val;
	__asm
	{
		pushad;
		mov ecx, nEcx;
		mov eax, GetTasksData_call;
		call eax;
		mov nEcx, eax;
		popad;
	};

	DWORD len = 0x7e;
	if (nEcx == 0)
	{
		return taskList;
	}

	DWORD num = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<BYTE>(nEcx + 0);
	if (num > 200)
	{
		return taskList;
	}

	for (size_t i = 0; i < num; i++)
	{
		DWORD objAddr = nEcx + i * len;
		StuObj obj;
		obj.mType = eStuObjType_任务;
		obj.mId32 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(objAddr + 0x1f);
		obj.mKillNum = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<WORD>(objAddr + 1);
		obj.maddr = (PULONG_PTR)objAddr;
		obj.mName = GetTaskName(obj.mId32);
		taskList.mData.push_back(obj);
	}





	return taskList;
}




static DWORD jieTask_c = GlobalGame::GetInstance()->CalcJiZhiContext(searchCode("xajh.exe", ".text", "8B4C24508B5424488B454C518B4D4453528B54245050A1????????518B48??52E8????????5E5F5BB0015D83C430C3", 0x16));
static DWORD jieTask_call = GlobalGame::GetInstance()->CalcCallPos(searchCode("xajh.exe", ".text", "8B4C24508B5424488B454C518B4D4453528B54245050A1????????518B48??52E8????????5E5F5BB0015D83C430C3", 0x20));
void DataQuery::jieTask(DWORD taskId)
{
	// TODO: 在此处添加实现代码.
	//8B 4C 24 50 8B 54 24 48 8B 45 4C 51 8B 4D 44 53 52 8B 54 24 50 50 A1 ?? ?? ?? ?? 51 8B 48 ?? 52 E8 ?? ?? ?? ?? 5E 5F 5B B0 01 5D 83 C4 30 C3 
	/*
	00ABECB4 | A1 10C05901             | mov eax,dword ptr ds:[0x159C010]                        |
	00ABECBA | 8B48 38                 | mov ecx,dword ptr ds:[eax+0x38]                         |
	*/
	__asm
	{

		//mov eax,

		push 0;
		push - 1;
		push 0;
		push 0;
		push 0;
		push taskId;
		mov ecx, jieTask_c;
		mov eax, jieTask_call;
		call eax;
	};
}

static DWORD WanChengTask_call = GlobalGame::GetInstance()->CalcCallPos(searchCode("xajh.exe", ".text", "8B81????????8B15????????8B0D????????535750A1????????528B15????????505152E8????????", 0x24));
void DataQuery::WanChengTask(DWORD taskId, StuObj npcObj)
{
	// TODO: 在此处添加实现代码.

	int height32 = (npcObj.mId64 & 0xffffffff00000000) >> 32;
	int low32 = npcObj.mId64 & 0x00000000ffffffff;
	DWORD taskNpcId = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(npcObj.maddr + 0x958));
	__asm
	{
		push 0;
		push - 1;
		push - 1;
		push height32;
		push low32;
		push taskNpcId;
		push taskId;
		mov ecx, taskNpcId;
		mov eax, WanChengTask_call;
		call eax;
		add esp, 0x1C;

	};


}

static DWORD GetKeJieTasks_Call1 = GlobalGame::GetInstance()->CalcCallPos(searchCode("xajh.exe", ".text", "8B90????????8954242CE8????????8B58??", 0xA));
static DWORD GetKeJieTasks_Call2 = GlobalGame::GetInstance()->CalcCallPos(searchCode("xajh.exe", ".text", "8B0D????????894C2468C78424D000000000000000E8????????", 0x15));
static DWORD GetKeJieTasks_Call3 = GlobalGame::GetInstance()->CalcCallPos(searchCode("xajh.exe", ".text", "83C4048B8424AC0000003BC6740950E8????????83C404", 0xf));
static DWORD GetKeJieTasks_Call4 = GlobalGame::GetInstance()->CalcCallPos(searchCode("xajh.exe", ".text", "8B4C242C8D9424A00000005253C68424D800000005E8????????", 0x15));
StuObjs DataQuery::GetKeJieTasks()
{
	// TODO: 在此处添加实现代码.
	DWORD push1 = 0;
	DWORD nEcx = 0;
	DWORD arrBase = 0;
	DWORD arrEnd = 0;
	StuObjs taskList;
	__asm
	{
		mov eax, GetKeJieTasks_Call1;
		call eax;
		mov eax, [eax + 0x30];
		mov push1, eax;

		mov eax, GetKeJieTasks_Call2;
		call eax;
		mov eax, [eax + 0x248];
		mov nEcx, eax;

	};

	DWORD arr[10] = { 0 };
	__asm
	{
		lea eax, arr;
		push eax;
		push push1;
		mov ecx, nEcx;
		mov eax, GetKeJieTasks_Call4;
		call eax;
	};
	arrBase = arr[3];
	arrEnd = arr[4];


	DWORD* selfPtr = (DWORD*)arrBase;
	while ((DWORD)selfPtr<= arrEnd)
	{
		DWORD objD =  *selfPtr;


		StuObj obj;
		obj.mType = eStuObjType_任务;
		obj.mId32 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<DWORD>(objD + 4);
		obj.maddr = (PULONG_PTR)objD;
		obj.mName = GetTaskName(obj.mId32);
		taskList.mData.push_back(obj);

		selfPtr++;
	}
	
	__asm 
	{
		push arrBase;
		mov eax, GetKeJieTasks_Call3;
		call eax;
		add esp, 4;
	};
	
	return taskList;
}
