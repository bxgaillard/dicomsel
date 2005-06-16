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
BIN    = rf$(EXESUFFIX)
DEPLIB = ..$(DIRSEP)libdicom$(DIRSEP)src$(DIRSEP)libdicom.a

include ../mkfiles/rules.mk

$(BIN): $(DEPLIB)

ifneq ($(OS),Windows)
    INCLUDES := $(INCLUDES) $(shell wx-config $(WXFLAGS) --cxxflags)
    LIBS     := $(LIBS) $(shell wx-config $(WXFLAGS) --libs core,net,base)
else
    #INCLUDES += -mthreads
    #LIBS     += -mthreads
    INCLUDES += -D__WXMSW__ -DNO_GCC_PRAGMA \
		-I..$(DIRSEP)wxwidgets$(DIRSEP)include
    LIBS     += -s -mwindows -Wl,--subsystem,windows \
		-L..$(DIRSEP)wxwidgets$(DIRSEP)lib -lwx_mswu_core-2.6 \
		-lwx_baseu_net-2.6 -lwx_baseu-2.6 -lwxregexu-2.6 \
		-lwxexpat-2.6 -lwxtiff-2.6 -lwxjpeg-2.6 -lwxpng-2.6 \
		-lwxzlib-2.6 -lunicows -lrpcrt4 -loleaut32 -lole32 -luuid \
		-lwinspool -lwinmm -lshell32 -lcomctl32 -lcomdlg32 -lctl3d32 \
		-ladvapi32 -lwsock32 -lgdi32
endif

# End of File
