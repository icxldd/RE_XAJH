#pragma once
#include "pch.h"



//����/��Ƶ����
void StartVoipChat(wchar_t* wxid,BOOL dwType);

void StartVoipChat2(wchar_t* wxid,BOOL dwType);

//�����ı���Ϣ
void SendWxTextMsg(wchar_t* wxid, wchar_t* msg);

//�����ı���Ϣ����ʾ��
void SendWxTextMsgDemo(wchar_t* wxid, wchar_t* msg);



//�������ݵ���ջ�в��ҵ��ú���
BOOL CopyDataInvokeFun(void* data,DWORD datasize,DWORD dwCall,DWORD dwEcx);




void ReadGlobalData();



void EachVectorData();


void EachListData();


void EachMapData();

void PatchCRC();