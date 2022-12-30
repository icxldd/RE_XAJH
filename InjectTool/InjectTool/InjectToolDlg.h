
// InjectToolDlg.h: 头文件
//

#include <string>
using namespace std;

#pragma once


// CInjectToolDlg 对话框
class CInjectToolDlg : public CDialogEx
{
// 构造
public:
	CInjectToolDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INJECTTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInject();



	DWORD ProcessNameFindID(PCWSTR ProcessName);

	//获取当前工作目录
	char* GetCurrentDirectry();

	//获取第一个dll文件
	string GetFirstDll();


	//获取卸载的dll模块句柄
	LPVOID GetDllModuleBase(DWORD dwPid);


	afx_msg void OnBnClickedUninject();
};
