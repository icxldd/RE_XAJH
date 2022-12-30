
// InjectToolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "InjectTool.h"
#include "InjectToolDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <direct.h>
#include <io.h>
#include <atlconv.h>
#define PROCESSNAME L"xajh.exe"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CInjectToolDlg 对话框



CInjectToolDlg::CInjectToolDlg(CWnd *pParent /*=nullptr*/)
	: CDialogEx(IDD_INJECTTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInjectToolDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInjectToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Inject, &CInjectToolDlg::OnBnClickedInject)
	ON_BN_CLICKED(IDC_UnInject, &CInjectToolDlg::OnBnClickedUninject)
END_MESSAGE_MAP()


// CInjectToolDlg 消息处理程序

BOOL CInjectToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu *pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInjectToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInjectToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CInjectToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//点击注入以后会响应
void CInjectToolDlg::OnBnClickedInject()
{
	//获取当前工作目录
	char *curdir = GetCurrentDirectry();

	//获取当前目录下的dll名称
	string dllname = GetFirstDll();

	//dll全路径
	string dllpath;
	dllpath.append(curdir);
	dllpath.append("\\");
	dllpath.append(dllname);



	//1.获取微信进程的PID
	DWORD dwPid = ProcessNameFindID(PROCESSNAME);

	//2.打开微信进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL)
	{
		MessageBoxA(0, "打开微信进程失败", "Tip", 0);
		return;
	}


	//3.在微信进程中申请一块内存
	LPVOID lpAddress = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (lpAddress == NULL)
	{
		MessageBoxA(0, "在微信进程中申请一块内存失败", "Tip", 0);
		return;
	}

	//4.写入要注入的dll的绝对路径到微信进程
	DWORD dwWrite = 0;
	BOOL bSuccess = WriteProcessMemory(hProcess, lpAddress, dllpath.c_str(), MAX_PATH, &dwWrite);
	if (bSuccess == FALSE)
	{
		MessageBoxA(0, "写入要注入的dll的绝对路径到微信进程失败", "Tip", 0);
		return;
	}

	//5.获取LoadLibraryA函数地址

	//5.1拿到kernel32的基地址
	HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
	FARPROC pLoadLibraryAddr = GetProcAddress(hKernel32, "LoadLibraryA");
	if (pLoadLibraryAddr == NULL)
	{
		MessageBoxA(0, "获取LoadLibraryA函数地址失败", "Tip", 0);
		return;
	}



	//6.用`CreateRemoteThread`远程线程注入dll

	HANDLE hRemote = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryAddr, lpAddress, 0, NULL);
	if (hRemote == NULL)
	{
		MessageBoxA(0, "CreateRemoteThread失败", "Tip", 0);
		return;
	}

	//7.关闭句柄
	CloseHandle(hProcess);

	CloseHandle(hRemote);
}




//************************************
// 函数名称: ProcessNameFindID
// 函数说明: 通过进程名称获取进程ID
// 作    者: 鬼手56
// 时    间: 2022/03/18
// 参    数: ProcessName
// 返 回 值: DWORD 进程ID
//************************************
DWORD CInjectToolDlg::ProcessNameFindID(PCWSTR ProcessName)
{
	//1.创建快照
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	//保存进程信息的结构体
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);


	//2.遍历快照

	if (Process32First(hSnapshot, &pe32))
	{
		do
		{
			//如果当前模块的进程和要找的进程名一致
			if (StrCmpW(pe32.szExeFile, ProcessName) == 0)
			{
				//找到了目标进程 返回进程ID
				return pe32.th32ProcessID;
			}

		} while (Process32Next(hSnapshot, &pe32));


	}
	return 0;

}

//************************************
// 函数名称: GetCurrentDirectry
// 函数说明: 获取当前工作目录
// 作    者: 鬼手56
// 时    间: 2022/03/18
// 参    数: 
// 返 回 值:  
//************************************
char *CInjectToolDlg::GetCurrentDirectry()
{
	char *dir = _getcwd(NULL, 0);

	return dir;
}




//************************************
// 函数名称: GetFirstDll
// 函数说明: 获取第一个dll文件
// 作    者: 鬼手56
// 时    间: 2022/03/18
// 参    数: 
// 返 回 值:  
//************************************
string CInjectToolDlg::GetFirstDll()
{
	//拼接要遍历的文件路径 当前目录下所有的文件
	string searchPath;
	searchPath.append(GetCurrentDirectry());
	searchPath.append("\\*");

	//文件信息结构体
	_finddata_t fileinfo;

	//获取第一个文件
	int hFile = _findfirst(searchPath.c_str(), &fileinfo);
	if (hFile == 0)
	{
		MessageBoxA(0, "查找第一个文件失败", "Tip", 0);
		return nullptr;
	}


	do
	{
		//过滤掉当前目录和上一级目录
		if (StrCmpA(".", fileinfo.name) == 0 || StrCmpA("..", fileinfo.name) == 0)
		{
			continue;
		}


		//判断文件后缀
		if (StrCmpA(PathFindExtensionA(fileinfo.name), ".dll") == 0)
		{
			string FileName = fileinfo.name;
			return FileName;
		}

	} while (_findnext(hFile, &fileinfo) == 0);


	return nullptr;

}


//************************************
// 函数名称: GetDllModuleBase
// 函数说明: 获取卸载的dll模块句柄
// 作    者: 鬼手56
// 时    间: 2022/03/18
// 参    数: dwPid
// 返 回 值: HMODULE 模块句柄
//************************************
LPVOID CInjectToolDlg::GetDllModuleBase(DWORD dwPid)
{
	//获取模块快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);


	//模块信息结构体
	MODULEENTRY32 ME32 = { 0 };
	ME32.dwSize = sizeof(MODULEENTRY32);


	if (Module32First(hSnap, &ME32))
	{
		do
		{

			//判断模块名称是否和注入的dll名称一致
			//获取到注入的dll的名称
			string dllname = GetFirstDll();
			USES_CONVERSION;
			if (StrCmpA(dllname.c_str(), W2A(ME32.szModule)) == 0)
			{
				//找到了要卸载的模块
				return ME32.modBaseAddr;
			}


		} while (Module32Next(hSnap, &ME32));
	}


	return NULL;

}

//卸载按钮按下的响应事件
void CInjectToolDlg::OnBnClickedUninject()
{
	//1.获取微信进程的PID
	DWORD dwPid = ProcessNameFindID(PROCESSNAME);

	//拿到要卸载模块的基地址
	LPVOID dwBase = GetDllModuleBase(dwPid);


	//2.打开微信进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL)
	{
		MessageBoxA(0, "打开微信进程失败", "Tip", 0);
		return;
	}



	//5.获取FreeLibrary函数地址

	//5.1拿到kernel32的基地址
	HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
	FARPROC pFreeLibrary = GetProcAddress(hKernel32, "FreeLibrary");
	if (pFreeLibrary == NULL)
	{
		MessageBoxA(0, "获取FreeLibrary函数地址失败", "Tip", 0);
		return;
	}


	//6.用`CreateRemoteThread`卸载dll 传入dll模块的句柄

	HANDLE hRemote = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFreeLibrary, dwBase, 0, NULL);
	if (hRemote == NULL)
	{
		MessageBoxA(0, "CreateRemoteThread失败", "Tip", 0);
		return;
	}

	//7.关闭句柄
	CloseHandle(hProcess);

	CloseHandle(hRemote);

}
