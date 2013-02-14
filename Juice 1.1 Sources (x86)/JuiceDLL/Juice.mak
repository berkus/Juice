# Microsoft Developer Studio Generated NMAKE File, Format Version 40001
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=Omega - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Omega - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Omega - Win32 Release" && "$(CFG)" != "Omega - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Juice.mak" CFG="Omega - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Omega - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Omega - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Omega - Win32 Debug"
CPP=cl.exe
MTL=mktyplib.exe
RSC=rc.exe

!IF  "$(CFG)" == "Omega - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "$(OUTDIR)\npJuice.dll"

CLEAN : 
	-@erase ".\WinRel\npJuice.dll"
	-@erase ".\WinRel\npwin.obj"
	-@erase ".\WinRel\Juice.obj"
	-@erase ".\WinRel\Juice.res"
	-@erase ".\WinRel\npJuice.lib"
	-@erase ".\WinRel\npJuice.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MT /W3 /GX /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MT /W3 /GX /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Juice.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Juice.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Juice.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386 /out:"WinRel/npJuice.dll"
# SUBTRACT LINK32 /debug
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)/npJuice.pdb"\
 /machine:I386 /def:".\Juice.def" /out:"$(OUTDIR)/npJuice.dll"\
 /implib:"$(OUTDIR)/npJuice.lib" 
DEF_FILE= \
	".\Juice.def"
LINK32_OBJS= \
	".\WinRel\npwin.obj" \
	".\WinRel\Juice.obj" \
	".\WinRel\Juice.res"

"$(OUTDIR)\npJuice.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Omega - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\npJuice.dll"

CLEAN : 
	-@erase ".\WinDebug\vc40.pdb"
	-@erase ".\WinDebug\vc40.idb"
	-@erase ".\WinDebug\npJuice.dll"
	-@erase ".\WinDebug\npwin.obj"
	-@erase ".\WinDebug\Juice.obj"
	-@erase ".\WinDebug\Juice.res"
	-@erase ".\WinDebug\npJuice.ilk"
	-@erase ".\WinDebug\npJuice.lib"
	-@erase ".\WinDebug\npJuice.exp"
	-@erase ".\WinDebug\npJuice.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Juice.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Juice.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Juice.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"WinDebug/npJuice.dll"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)/npJuice.pdb" /debug\
 /machine:I386 /def:".\Juice.def" /out:"$(OUTDIR)/npJuice.dll"\
 /implib:"$(OUTDIR)/npJuice.lib" 
DEF_FILE= \
	".\Juice.def"
LINK32_OBJS= \
	".\WinDebug\npwin.obj" \
	".\WinDebug\Juice.obj" \
	".\WinDebug\Juice.res"

"$(OUTDIR)\npJuice.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Omega - Win32 Release"
# Name "Omega - Win32 Debug"

!IF  "$(CFG)" == "Omega - Win32 Release"

!ELSEIF  "$(CFG)" == "Omega - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\npwin.cpp
DEP_CPP_NPWIN=\
	".\npapi.h"\
	".\npupp.h"\
	".\jri.h"\
	".\jri_md.h"\
	

"$(INTDIR)\npwin.obj" : $(SOURCE) $(DEP_CPP_NPWIN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Juice.def

!IF  "$(CFG)" == "Omega - Win32 Release"

!ELSEIF  "$(CFG)" == "Omega - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Juice.rc
DEP_RSC_JUICE=\
	".\Juice.hex"\
	
NODEP_RSC_JUICE=\
	".\Omega.rc2"\
	

"$(INTDIR)\Juice.res" : $(SOURCE) $(DEP_RSC_JUICE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Juice.cpp
DEP_CPP_JUICE_=\
	".\npapi.h"\
	".\jri.h"\
	".\jri_md.h"\
	

"$(INTDIR)\Juice.obj" : $(SOURCE) $(DEP_CPP_JUICE_) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
