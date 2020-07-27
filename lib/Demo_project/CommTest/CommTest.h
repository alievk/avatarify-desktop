// CommTest.h : main header file for the COMMTEST application
//

#if !defined(AFX_COMMTEST_H__85BEB089_7A69_4442_90EB_2A8A89973A18__INCLUDED_)
#define AFX_COMMTEST_H__85BEB089_7A69_4442_90EB_2A8A89973A18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCommTestApp:
// See CommTest.cpp for the implementation of this class
//

class CCommTestApp : public CWinApp
{
public:
	CCommTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCommTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMTEST_H__85BEB089_7A69_4442_90EB_2A8A89973A18__INCLUDED_)
