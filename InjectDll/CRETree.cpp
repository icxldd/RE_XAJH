#include "CRETree.h"

void __stdcall TreeEach()
{



}

//call    ??E?$_Tree_iterator@V?$_Tree_val@U?$_Tree_simple_types@U?$pair@$$CBHUPerson@@@std@@@std@@@std@@@std@@QAEAAV01@XZ ; this = ite
__declspec(naked) void __stdcall TreeJiaJia()
{
	__asm 
	{
		pushad;
		push    esi;
		mov     esi, ecx;
		call TreeJiaJia2;
		mov     eax, esi;
		pop     esi;
		popad;
		retn;
	}
}

__declspec(naked) void __stdcall TreeJiaJia2()
{
	__asm
	{

		push    esi;
		mov     esi, ecx;
		mov     edx, [esi];
		mov     eax, [edx + 8];
		cmp     byte ptr[eax + 0Dh], 0;
		jz      short loc_CF4CFF;
		mov     eax, [edx + 4];
		cmp     byte ptr[eax + 0Dh], 0;
		jnz     short loc_CF4D08;

	loc_CF4CE7:
		cmp     edx, [eax + 8];
		jnz     short loc_CF4D08;
		mov[esi], eax;
		mov     edx, eax;
		mov     eax, [eax + 4];
		cmp     byte ptr[eax + 0Dh], 0;
		jz      short loc_CF4CE7;
		mov[esi], eax;
		mov     eax, esi;
		pop     esi;
		retn;

	loc_CF4CFF:
		push    eax;
		call GetMinTree;
		add     esp, 4;

	loc_CF4D08:

		mov[esi], eax;
		mov     eax, esi;
		pop     esi;
		retn;


	}
}


__declspec(naked) void __stdcall GetMinTree()
{

	__asm
	{
		push    ebp;
		mov     ebp, esp;
		mov     eax, [ebp + 8];
		mov     ecx, [eax]; ecx = pRightNode����ڵ�
			cmp     byte ptr[ecx + 0Dh], 0; _Isnil�ж�ecx�ڵ��Ƿ���Ҷ�ӽڵ㣬�������߷��ش˽ڵ�
			jnz     short loc_CF542E;
		xchg    ax, ax;
	loc_CF5420:
		mov     eax, [ecx]; eax = ȡecx�ڵ����ڵ�
			mov     edx, ecx; edx = ecx = pRightNode����ڵ�
			mov     ecx, eax; ecx = eax = ȡecx�ڵ����ڵ�
			cmp     byte ptr[eax + 0Dh], 0; ��pRightNode����ڵ㡿�Ƿ���Ҷ�ӽڵ㣬��������������ȡ��ǰ�ڵ����ڵ�
			jz      short loc_CF5420; eax = ȡecx�ڵ����ڵ�
			mov     eax, edx;
	loc_CF542E:
		pop     ebp;
		retn;
	}
}
