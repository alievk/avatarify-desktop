// CommTestDlg.h : header file
//

#if !defined(AFX_COMMTESTDLG_H__7FFEC2BB_AD0C_4D71_A44F_17C05A9DF48F__INCLUDED_)
#define AFX_COMMTESTDLG_H__7FFEC2BB_AD0C_4D71_A44F_17C05A9DF48F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCommTestDlg dialog

class CCommTestDlg : public CDialog
{
// Construction
public:
	static DWORD WINAPI TestThread(LPVOID lpParam);
	CCommTestDlg(CWnd* pParent = NULL);	// standard constructor

private:
	HANDLE m_hCommDevice;
	HANDLE m_hCommEvent;

// Dialog Data
	//{{AFX_DATA(CCommTestDlg)
	enum { IDD = IDD_COMMTEST_DIALOG };
	CListBox	m_LogList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCommTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenDevice();
	afx_msg void OnCloseDevice();
	afx_msg void OnQueryEvent();
	afx_msg void OnClearEvent();
	afx_msg void OnSetEvent();
	afx_msg void OnCreateThread();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMTESTDLG_H__7FFEC2BB_AD0C_4D71_A44F_17C05A9DF48F__INCLUDED_)
