#pragma once
#define WxHookGetLog 0x7005D0


#define WxMiniHookGetLog 0x19F8DB0


/*
78B51DC0 | 55                       | push ebp                                                      |
78B51DC1 | 8BEC                     | mov ebp,esp                                                   |
78B51DC3 | 8B45 08                  | mov eax,dword ptr ss:[ebp+8]                                  |
*/
#define Offset0x700DC0 0x701DC1

//语音聊天的偏移
#define WxStartVoipCall1 0xC00B0
#define WxStartVoipCall2 0x9DDC0
#define WxStartVoipCall3 0xBF6220
#define WxStartVoipEcx 0x2255DE8


//发送文本消息的偏移
#define WxSendTextMsg 0x4BE7B0





#define GlobalVarData 0x1D0B98

#define WorldOffset 0x1D0B98
#define ListPageThisOffset 0xD8
#define MapPageThisOffset 0xDC
#define InitVectorCallOffset 0x58C9

//edi = REStruct.exe+1D0B98	dword ptr ds:[edi+D8]=[012FFD44]=<&const CReListDlg::`vftable'>



//00C16DCC | 898F DC000000 | mov dword ptr ds : [edi + DC] , ecx | REStructDlg.cpp : 189


#define Jizhi 0xE6A5F8

