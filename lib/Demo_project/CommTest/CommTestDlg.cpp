// CommTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CommTest.h"
#include "CommTestDlg.h"

#include <winioctl.h>
#include "..\include\iocontrol.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommTestDlg dialog

CCommTestDlg::CCommTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hCommEvent = NULL;
	m_hCommDevice = NULL;
}

void CCommTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommTestDlg)
	DDX_Control(pDX, IDC_LIST, m_LogList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCommTestDlg, CDialog)
	//{{AFX_MSG_MAP(CCommTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_DEVICE, OnOpenDevice)
	ON_BN_CLICKED(IDC_CLOSE_DEVICE, OnCloseDevice)
	ON_BN_CLICKED(IDC_QUERY_EVENT, OnQueryEvent)
	ON_BN_CLICKED(IDC_CLEAR_EVENT, OnClearEvent)
	ON_BN_CLICKED(IDC_SET_EVENT, OnSetEvent)
	ON_BN_CLICKED(IDC_CREATE_THREAD, OnCreateThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommTestDlg message handlers

BOOL CCommTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCommTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCommTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCommTestDlg::OnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	m_hCommDevice = CreateFile(
			"\\\\.\\CommDriverDev", 
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ, NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	if(m_hCommDevice == INVALID_HANDLE_VALUE)
	{
		int error = GetLastError();
		m_LogList.AddString("Please install the device driver and start it firstly!");
	}
	else
		m_LogList.AddString("Successfully opened the device");

	DWORD dwReturn;

	m_hCommEvent = CreateEvent(NULL, false, false, NULL);
	
	//download event object to device driver
	DeviceIoControl(m_hCommDevice, 
			IO_REFERENCE_EVENT, 
			(LPVOID) m_hCommEvent,
			0, 
			NULL, 
			0, 
			&dwReturn, 
			NULL);
}

void CCommTestDlg::OnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	DWORD dwReturn;

	if(m_hCommDevice)
	{
		if(m_hCommEvent)
		{
			DeviceIoControl(m_hCommDevice, 
				IO_DEREFERENCE_EVENT, 
				NULL,
				0, 
				NULL, 
				0, 
				&dwReturn, 
				NULL);
			CloseHandle(m_hCommEvent);
		}

		CloseHandle(m_hCommDevice);
	}
}


void CCommTestDlg::OnQueryEvent() 
{
	// TODO: Add your control notification handler code here
	DWORD	dwReturn;
	ULONG	dwState;

	if(m_hCommEvent)
	{
		DeviceIoControl(m_hCommDevice, 
				IO_QUERY_EVENT_STATE, 
				NULL,
				0, 
				(LPVOID) &dwState, 
				sizeof(dwState), 
				&dwReturn, 
				NULL);
		if(dwState)
			m_LogList.AddString("Event signaled");
		else
			m_LogList.AddString("Event non signaled");
	}
}

void CCommTestDlg::OnClearEvent() 
{
	// TODO: Add your control notification handler code here
	DWORD	dwReturn;

	if(m_hCommEvent)
	{
		DeviceIoControl(m_hCommDevice, 
				IO_CLEAR_EVENT, 
				NULL,
				0, 
				NULL, 
				0, 
				&dwReturn, 
				NULL);
	}
}

void CCommTestDlg::OnSetEvent() 
{
	// TODO: Add your control notification handler code here
	DWORD	dwReturn;

	if(m_hCommEvent)
	{
		DeviceIoControl(m_hCommDevice, 
				IO_SET_EVENT, 
				NULL,
				0, 
				NULL, 
				0, 
				&dwReturn, 
				NULL);
	}
}

DWORD WINAPI CCommTestDlg::TestThread(LPVOID lpParam)
{
	CCommTestDlg* pThis = (CCommTestDlg*) lpParam;
	
	if(pThis->m_hCommEvent)
	{
		WaitForSingleObject(pThis->m_hCommEvent, INFINITE);
		AfxMessageBox("Get the event!!!");
	}

	return true;
}

void CCommTestDlg::OnCreateThread() 
{
	// TODO: Add your control notification handler code here
	DWORD dwThreadID;

	HANDLE hThread = CreateThread(NULL, 
			0, 
			TestThread, 
			this, 
			0, 
			&dwThreadID);

	CloseHandle(hThread);
}
