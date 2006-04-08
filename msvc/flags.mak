# ----------------------------------------------------------------------------
#
# DicomSel: DICOM Image Selector
#
# Copyright (C) 2005, 2006 IRCAD
# Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
#
# ----------------------------------------------------------------------------
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc., 59
# Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# ----------------------------------------------------------------------------


# You can edit these variables as your convenience

# For a release build
CFLAGS = /Ox /W4 /Wp64
CXXFLAGS = /Ox /W4 /Wp64 /EHsc
CPPFLAGS = /DNDEBUG /DHAVE_CONFIG_H /I$(TOPDIR)\msvc

# For a debug build
#CFLAGS = /W4 /Wp64 /Zi
#CXXFLAGS = /W4 /Wp64 /EHsc /Zi
#CPPFLAGS = /DDEBUG /DHAVE_CONFIG_H /I$(TOPDIR)\msvc

# Path to the wxWidgets main directory
WX_PATH = $(TOPDIR)\..\wxWidgets

# Please do not change anything below this line

CC   = cl /TC $(CLFLAGS)
CXX  = cl /TP $(CLFLAGS)
LINK = link /NOLOGO
RC   = rc
CLFLAGS = /nologo /ML

WX_DEFS     = /DWIN32 /DwxNO_EXCEPTIONS /DwxNO_RTTI /DwxNO_THREADS
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
