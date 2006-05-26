# ----------------------------------------------------------------------------
#
# DicomSel: DICOM Image Selector
#
# Copyright (C) 2005, 2006 IRCAD
# Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
#
# ----------------------------------------------------------------------------
#
# << LICENSE >>
#
# ----------------------------------------------------------------------------


# You can edit these variables as your convenience

# For a release build
CFLAGS = /Ox /W4 /Wp64
CXXFLAGS = /Ox /W4 /Wp64 /EHsc
CPPFLAGS = /DNDEBUG /DHAVE_CONFIG_H /I$(TOPDIR)\msvc /I$(TOPDIR)

# For a debug build
#CFLAGS = /W4 /Wp64 /Zi
#CXXFLAGS = /W4 /Wp64 /EHsc /Zi
#CPPFLAGS = /DDEBUG /DHAVE_CONFIG_H /I$(TOPDIR)\msvc /I$(TOPDIR)

# Path to the wxWidgets main directory
WX_PATH = $(TOPDIR)\..\wxWidgets

# Please do not change anything below this line

# DCMTK
DCMTK_VERSION = 3.5.4
DCMTK_DIR_NIX = 3rdparty/dcmtk-3.5.4
DCMTK_DIR     = $(DCMTK_DIR_NIX:/=\)
OPENJPEG_VERSION = 1.0
OPENJPEG_DIR_NIX = 3rdparty/openjpeg-1.0
OPENJPEG_DIR     = $(OPENJPEG_DIR_NIX:/=\)

CC   = cl /TC $(CLFLAGS)
CXX  = cl /TP $(CLFLAGS)
LINK = link /NOLOGO
RC   = rc
#CLFLAGS = /nologo /ML
CLFLAGS = /nologo /MT
LDFLAGS = /OPT:REF,ICF

WX_DEFS     = /DWIN32 /DwxNO_EXCEPTIONS /DwxNO_RTTI
WX_INCLUDES = /I$(WX_PATH)\include /I$(WX_PATH)\lib\vc_lib\msw
WX_RC_INCLS = /i$(WX_PATH)\include /i$(WX_PATH)\lib\vc_lib\msw
WX_LIBS     = /LIBPATH:$(WX_PATH)\lib\vc_lib wxbase26.lib wxmsw26_core.lib \
	      kernel32.lib user32.lib gdi32.lib comdlg32.lib shell32.lib \
	      comctl32.lib ole32.lib advapi32.lib

.SUFFIXES:
.SUFFIXES: .c .cpp .obj .rc .res

.c.obj:   ; $(CC)  $(CFLAGS)   $(CPPFLAGS) $(DEFS) $(INCLUDES) /c $< /Fo$@
.cpp.obj: ; $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(DEFS) $(INCLUDES) /c $< /Fo$@
.rc.res:  ; $(RC) $(RC_INCLS) /fo$@ $<

# End of File
