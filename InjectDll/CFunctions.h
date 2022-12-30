#pragma once
#include "pch.h"



//语音/视频聊天
void StartVoipChat(wchar_t* wxid,BOOL dwType);

void StartVoipChat2(wchar_t* wxid,BOOL dwType);

//发送文本消息
void SendWxTextMsg(wchar_t* wxid, wchar_t* msg);

//发送文本消息调用示例
void SendWxTextMsgDemo(wchar_t* wxid, wchar_t* msg);



//拷贝数据到堆栈中并且调用函数
BOOL CopyDataInvokeFun(void* data,DWORD datasize,DWORD dwCall,DWORD dwEcx);




void ReadGlobalData();



void EachVectorData();


void EachListData();


void EachMapData();

void PatchCRC();