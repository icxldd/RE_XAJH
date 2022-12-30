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

		BagData.mData.push_back(obj);
		//DebugPrint("物品数组对象[%d]：%p,物品数量:%d\r\n", i, adddr, GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>((ULONG_PTR)adddr + BeiBaoWupinLenOffset));
		addr++;
	}


	return BagData;
}

string GetNpcName(ULONG_PTR thisEcx)
{
	ULONG_PTR adress = GlobalGame::GetInstance()->GetNpcNameCall();
	ULONG_PTR dret = 0;
	__asm {
		mov ecx, thisEcx;
		call adress;
		mov dret, eax;
	};
	//DebugPrint("名字：%p,ecx:%p", dret, thisEcx);
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
			obj.mType = eStuObjType_对象;
			obj.maddr = (PULONG_PTR)ObjData;
			obj.mZuoBiao.x = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(ObjData + NPCWeizhiOffset + 0);//x坐标加256
			obj.mZuoBiao.z = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(ObjData + NPCWeizhiOffset + 4);
			obj.mZuoBiao.y = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<float>(ObjData + NPCWeizhiOffset + 8);
			obj.mId64 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<INT64>(ObjData + NPCIdOffset);
			//DebugPrint("---------对象地址%p,ID:%llx\r\n", ObjData, id);
			obj.mName = GetNpcName(ObjData);

			objs.mData.push_back(obj);
			ObjPoint = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(ObjPoint + 0x0);
		}


	}
	return objs;
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


void DataQuery::Test()
{
	//[[[[[0x01597F50]+0x30]+0xc]+0x4]+0x40]
	ULONG_PTR  jizhi = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(0x01597F50);
	ULONG_PTR o1 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(jizhi + 0x30);
	ULONG_PTR o2 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o1 + 0xc);
	ULONG_PTR o3 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o2 + 0x4);
	//ULONG_PTR o5 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o3 + 0x40);
	DWORD dd = 0x67EB2C58;
	GlobalGame::GetInstance()->GetMemoryOp()->WriteMemory<ULONG_PTR>(o3 + 0x40, dd);
	ULONG_PTR o5 = GlobalGame::GetInstance()->GetMemoryOp()->ReadMemory<ULONG_PTR>(o3 + 0x40);
	//68432EF8
	DebugPrint("%p", o5);
	__asm {

		push dd;
		push 0;
		push 0;
		push 0;
		push 7;
		push 0xa;
		mov ecx, 0x61124360;
		mov eax, 0x06BDA00;
		call eax;
	};
	// TODO: 在此处添加实现代码.
}
