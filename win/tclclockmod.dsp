# Microsoft Developer Studio Project File - Name="tclclockmod" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=tclclockmod - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "makefile.vc".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "makefile.vc" CFG="tclclockmod - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "tclclockmod - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "tclclockmod - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "tclclockmod - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f makefile.vc"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "tclclockmod.dll"
# PROP BASE Bsc_Name "tclclockmod.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "nmake -nologo -f makefile.vc TCLDIR=E:\tcl MSVCDIR=IDE"
# PROP Rebuild_Opt "-a"
# PROP Target_File "Release\tclclockmod.dll"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "tclclockmod - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f makefile.vc"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "tclclockmod.exe"
# PROP BASE Bsc_Name "tclclockmod.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "nmake -nologo -f makefile.vc OPTS=symbols TCLDIR=E:\tcl MSVCDIR=IDE"
# PROP Rebuild_Opt "-a"
# PROP Target_File "Debug\tclclockmodg.dll"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF

# Begin Target

# Name "tclclockmod - Win32 Release"
# Name "tclclockmod - Win32 Debug"

!IF  "$(CFG)" == "tclclockmod - Win32 Release"

!ELSEIF  "$(CFG)" == "tclclockmod - Win32 Debug"

!ENDIF

ROOT=..

# Begin Group "generic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=$(ROOT)\generic\tclClock.c
# End Source File
# Begin Source File

SOURCE=$(ROOT)\generic\tclClockFmt.c
# End Source File
# Begin Source File

SOURCE=$(ROOT)\generic\tclClockMod.c
# End Source File
# Begin Source File

SOURCE=$(ROOT)\generic\tclClockModInt.c
# End Source File
# Begin Source File

SOURCE=$(ROOT)\generic\tclClockModInt.h
# End Source File
# Begin Source File

SOURCE=$(ROOT)\generic\tclDate.c
# End Source File
# Begin Source File

SOURCE=$(ROOT)\generic\tclDate.h
# End Source File
# Begin Source File

SOURCE=$(ROOT)\generic\tclStrIdxTree.c
# End Source File
# Begin Source File

SOURCE=$(ROOT)\generic\tclStrIdxTree.h
# End Source File
# End Group

# Begin Group "win"

# PROP Default_Filter ""
# Begin Group "vc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=$(ROOT)\win\makefile.vc
# End Source File
# Begin Source File

SOURCE=$(ROOT)\win\nmakehlp.c
# End Source File
# Begin Source File

SOURCE=$(ROOT)\win\pkg.vc
# End Source File
# Begin Source File

SOURCE=$(ROOT)\win\rules.vc
# End Source File
# Begin Source File

SOURCE=$(ROOT)\win\tclclockmod.rc
# End Source File
# End Group
# Begin Source File

SOURCE=$(ROOT)\win\README.txt
# End Source File
# End Group
# Begin Source File

SOURCE=$(ROOT)\license.terms
# End Source File
# Begin Source File

SOURCE=$(ROOT)\README
# End Source File
# End Target
# End Project
