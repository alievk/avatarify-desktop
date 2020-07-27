# Microsoft Developer Studio Project File - Name="CommDriver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CommDriver - Win32 Checked
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CommDriver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CommDriver.mak" CFG="CommDriver - Win32 Checked"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CommDriver - Win32 Free" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CommDriver - Win32 Checked" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CommDriver - Win32 Free"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Free"
# PROP BASE Intermediate_Dir "Free"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Free"
# PROP Intermediate_Dir "Free"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /W3 /WX /Oy /Gy /D "WIN32" /Oxs /c
# ADD CPP /nologo /Gz /W3 /WX /Oy /Gy /I "$(BASEDIR)\inc\ddk" /I "$(BASEDIR)\inc" /I "$(CPU)\\" /I "." /I "$(DRIVERWORKS)\..\common\include\stl" /I "$(BASEDIR)\inc\ddk\wdm" /I "$(BASEDIR)\inc\win98" /FI"$(BASEDIR)\inc\warning.h" /D WIN32=100 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x0400 /D WIN32_LEAN_AND_MEAN=1 /D DEVL=1 /D FPO=1 /D "_IDWBUILD" /D "NDEBUG" /D _DLL=1 /D _X86_=1 /D $(CPU)=1 /FR /Oxs /Zel -cbstring /QIfdiv- /QIf /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "$(BASEDIR)\inc" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"objfre\i386/CommDriver.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /machine:IX86
# ADD LINK32 int64.lib ntoskrnl.lib hal.lib /nologo /base:"0x10000" /version:4.0 /entry:"DriverEntry" /machine:IX86 /nodefaultlib /out:"obj\i386\Free\CommDriver.sys" /libpath:"$(BASEDIR)\libfre\i386" /libpath:"$(BASEDIR)\\" /libpath:"$(BASEDIR)\lib\i386\free" /driver /debug:MINIMAL /IGNORE:4001,4037,4039,4065,4070,4078,4087,4089,4096,4210 /MERGE:_PAGE=PAGE /MERGE:_TEXT=.text /SECTION:INIT,d /MERGE:.rdata=.text /FULLBUILD /RELEASE /FORCE:MULTIPLE /OPT:REF /align:0x20 /osversion:4.00 /subsystem:native
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Generating SoftICE Symbol file CommDriver.nms
PostBuild_Cmds=E:\PROGRA~1\NuMega\DRIVER~1\SoftICE\nmsym.exe /translate:source,package,always obj\i386\Free\CommDriver.sys	copy obj\i386\Free\CommDriver.sys ..\Bin
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CommDriver - Win32 Checked"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Checked"
# PROP BASE Intermediate_Dir "Checked"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Checked"
# PROP Intermediate_Dir "Checked"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /W3 /Zi /Od /D "WIN32" /YX /FD /c
# ADD CPP /nologo /Gz /W3 /Z7 /Oi /Gy /I "$(BASEDIR)\inc\ddk" /I "$(BASEDIR)\inc" /I "$(CPU)\\" /I "." /I "$(DRIVERWORKS)\..\common\include\stl" /I "$(BASEDIR)\inc\ddk\wdm" /I "$(BASEDIR)\inc\win98" /FI"$(BASEDIR)\inc\warning.h" /D WIN32=100 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x0400 /D WIN32_LEAN_AND_MEAN=1 /D DBG=1 /D DEVL=1 /D FPO=0 /D "NDEBUG" /D _DLL=1 /D _X86_=1 /D $(CPU)=1 /FR /YX /FD /Zel -cbstring /QIfdiv- /QIf /GF /c
# ADD BASE MTL /nologo /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "$(BASEDIR)\inc"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"objchk\i386\CommDriver.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /machine:IX86
# ADD LINK32 int64.lib ntoskrnl.lib hal.lib /nologo /base:"0x10000" /version:4.0 /entry:"DriverEntry" /incremental:no /debug /machine:IX86 /nodefaultlib /out:"obj\i386\Checked\CommDriver.sys" /libpath:"$(BASEDIR)\libchk\i386" /libpath:"$(BASEDIR)\lib\i386\checked" /driver /debug:FULL /IGNORE:4001,4037,4039,4065,4078,4087,4089,4096,4210 /MERGE:_PAGE=PAGE /MERGE:_TEXT=.text /SECTION:INIT,d /MERGE:.rdata=.text /FULLBUILD /RELEASE /FORCE:MULTIPLE /OPT:REF /align:0x20 /osversion:4.00 /subsystem:native
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Generating SoftICE Symbol file CommDriver.nms
PostBuild_Cmds=E:\PROGRA~1\NuMega\DRIVER~1\SoftICE\nmsym.exe /translate:source,package,always obj\i386\Checked\CommDriver.sys	copy obj\i386\Checked\CommDriver.sys ..\Bin
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "CommDriver - Win32 Free"
# Name "CommDriver - Win32 Checked"
# Begin Group "Source Files"

# PROP Default_Filter ".c;.cpp"
# Begin Source File

SOURCE=.\CommDriver.c
DEP_CPP_COMMD=\
	"..\..\NTDDK\inc\alpharef.h"\
	"..\..\ntddk\inc\basetsd.h"\
	"..\..\ntddk\inc\bugcodes.h"\
	"..\..\ntddk\inc\ddk\ntddk.h"\
	"..\..\ntddk\inc\guiddef.h"\
	"..\..\NTDDK\inc\ia64reg.h"\
	"..\..\ntddk\inc\ntdef.h"\
	"..\..\ntddk\inc\ntiologc.h"\
	"..\..\ntddk\inc\ntstatus.h"\
	"..\Include\IoControl.h"\
	".\CommDriver.h"\
	

!IF  "$(CFG)" == "CommDriver - Win32 Free"

!ELSEIF  "$(CFG)" == "CommDriver - Win32 Checked"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\CommDriver.h
# End Source File
# Begin Source File

SOURCE=..\Include\IoControl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ".rc;.mc"
# End Group
# End Target
# End Project
