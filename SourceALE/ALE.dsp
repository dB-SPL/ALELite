# Microsoft Developer Studio Project File - Name="ALE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ALE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ALE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ALE.mak" CFG="ALE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ALE - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ALE - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ALE - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\HTML Help Workshop\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 dsound.lib dxguid.lib htmlhelp.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\Program Files\HTML Help Workshop\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ALE - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\Program Files\HTML Help Workshop\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dsound.lib dxguid.lib htmlhelp.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\HTML Help Workshop\lib"

!ENDIF 

# Begin Target

# Name "ALE - Win32 Release"
# Name "ALE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ALE.cpp
# End Source File
# Begin Source File

SOURCE=.\ALE.rc
# End Source File
# Begin Source File

SOURCE=.\ALEDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ALEView.cpp
# End Source File
# Begin Source File

SOURCE=.\AMDDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\AMDText.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelList.cpp
# End Source File
# Begin Source File

SOURCE=.\CircBufI.cpp
# End Source File
# Begin Source File

SOURCE=.\CircBufO.cpp
# End Source File
# Begin Source File

SOURCE=.\Config.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlgfloat.cpp
# End Source File
# Begin Source File

SOURCE=.\HDAudio.cpp
# End Source File
# Begin Source File

SOURCE=.\Icom.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherAddrDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherAddrList.cpp
# End Source File
# Begin Source File

SOURCE=.\Radio.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectIndivAddr.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectRxChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectSelfAddr.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectSoundAddrs.cpp
# End Source File
# Begin Source File

SOURCE=.\SelfAddrDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelfAddrList.cpp
# End Source File
# Begin Source File

SOURCE=.\SignalQualityDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SlotAddr.cpp
# End Source File
# Begin Source File

SOURCE=.\SlotList.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SunTimes.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ALE.h
# End Source File
# Begin Source File

SOURCE=.\ALEDoc.h
# End Source File
# Begin Source File

SOURCE=.\ALEView.h
# End Source File
# Begin Source File

SOURCE=.\AMDDisplay.h
# End Source File
# Begin Source File

SOURCE=.\AMDText.h
# End Source File
# Begin Source File

SOURCE=.\ChannelDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChannelList.h
# End Source File
# Begin Source File

SOURCE=.\CircBuf.h
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\Dlgfloat.h
# End Source File
# Begin Source File

SOURCE=.\HDAudio.h
# End Source File
# Begin Source File

SOURCE=.\Icom.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\OtherAddrDlg.h
# End Source File
# Begin Source File

SOURCE=.\OtherAddrList.h
# End Source File
# Begin Source File

SOURCE=.\Radio.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "ALE - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=ALE
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ALE - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=ALE
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SelectIndivAddr.h
# End Source File
# Begin Source File

SOURCE=.\SelectRxChannel.h
# End Source File
# Begin Source File

SOURCE=.\SelectSelfAddr.h
# End Source File
# Begin Source File

SOURCE=.\SelectSoundAddrs.h
# End Source File
# Begin Source File

SOURCE=.\SelfAddrDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelfAddrList.h
# End Source File
# Begin Source File

SOURCE=.\SignalQualityDlg.h
# End Source File
# Begin Source File

SOURCE=.\SlotAddr.h
# End Source File
# Begin Source File

SOURCE=.\SlotList.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SunTimes.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ALE.ico
# End Source File
# Begin Source File

SOURCE=.\res\ALE.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ALEDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\movedownd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\movedownf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\movedownu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\movedownx.bmp
# End Source File
# Begin Source File

SOURCE=.\res\moveupd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\moveupf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\moveupu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\moveupx.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ALE.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
