; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCommTestDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CommTest.h"

ClassCount=2
Class1=CCommTestApp
Class2=CCommTestDlg

ResourceCount=4
Resource2=IDR_MAINFRAME
Resource3=IDD_COMMTEST_DIALOG
Resource4=IDD_COMMTEST_DIALOG (English (U.S.))

[CLS:CCommTestApp]
Type=0
HeaderFile=CommTest.h
ImplementationFile=CommTest.cpp
Filter=N

[CLS:CCommTestDlg]
Type=0
HeaderFile=CommTestDlg.h
ImplementationFile=CommTestDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_QUERY_EVENT



[DLG:IDD_COMMTEST_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CCommTestDlg

[DLG:IDD_COMMTEST_DIALOG (English (U.S.))]
Type=1
Class=CCommTestDlg
ControlCount=8
Control1=IDC_OPEN_DEVICE,button,1342242817
Control2=IDC_CLOSE_DEVICE,button,1342242816
Control3=IDC_QUERY_EVENT,button,1342242817
Control4=IDC_LIST,listbox,1353777409
Control5=IDC_SET_EVENT,button,1342242817
Control6=IDC_CLEAR_EVENT,button,1342242817
Control7=IDC_CREATE_THREAD,button,1342242817
Control8=IDC_STATIC,static,1342308352

