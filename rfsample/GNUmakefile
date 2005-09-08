# ----------------------------------------------------------------------------
#
# DicomSel: DICOM Image Selector
#
# Copyright (C) 2005 IRCAD
# Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
#
# ----------------------------------------------------------------------------
#
# << LICENCE >>
#
# ----------------------------------------------------------------------------


TOPDIR = ..
BIN    = rf

include ../mkfiles/rules.mk

$(BIN): $(DEPLIB)

ifneq ($(TARGET_OS),Windows)
    INCLUDES := $(INCLUDES) $(shell wx-config $(WXFLAGS) --cxxflags)
    LIBS     := $(LIBS) $(shell wx-config $(WXFLAGS) --libs core,net,base)
    REZ      := $(shell wx-config --rezflags)
else
    INCLUDES += -D__WXMSW__ -DNO_GCC_PRAGMA \
		-I..$(DIRSEP)3rdparty$(DIRSEP)wxmsw$(DIRSEP)include
    LIBS     += -mwindows -Wl,--subsystem,windows:4.0 \
		-L..$(DIRSEP)3rdparty$(DIRSEP)wxmsw$(DIRSEP)lib \
		-lwx_msw_core-2.6 -lwx_base-2.6 -lrpcrt4 -loleaut32 -lole32 \
		-luuid -lwinspool -lwinmm -lshell32 -lcomctl32 -lcomdlg32 \
		-lctl3d32 -ladvapi32 -lwsock32 -lgdi32
    REZ       =
endif

# End of File
