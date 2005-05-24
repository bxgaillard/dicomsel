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


# OS-specific Definitions
ifneq ($(OS),Windows)
    # Programs
    CC     ?= gcc
    CXX    ?= g++
    RM     ?= rm -f
    AR     ?= ar
    RANLIB ?= ranlib

    # Compiling Flags
    CFLAGS   ?= -march=$(shell uname -m) -O2 -fomit-frame-pointer -pipe
    CFLAGS   := $(CFLAGS)
    CXXFLAGS ?= $(CFLAGS)
    CXXFLAGS := $(CXXFLAGS)

    # Additional Flags
    EXESUFFIX =

    # Windows Resources
    RCFILES =
else
    # Programs
    CC     ?= gcc.exe
    CXX    ?= g++.exe
    RC     ?= windres.exe
    RM     ?= rm.exe -f
    AR     ?= ar.exe
    RANLIB ?= ranlib.exe

    # Compiling Flags
    CFLAGS   ?= -march=i386 -mtune=i686 -O2 -fomit-frame-pointer -pipe
    CXXFLAGS ?= $(CFLAGS)
    CXXFLAGS := $(CXXFLAGS)
    CFLAGS   += -fno-pcc-struct-return
    CXXFLAGS += -fno-pcc-struct-return

    # Additional Flags
    EXESUFFIX = .exe

    # Windows Resources
    RCFILES := $(wildcard *.rc)
endif

# Custom Additions to Flags
WARN      = -Wall -Wextra
CFLAGS   += $(WARN)
CXXFLAGS += -fno-check-new -fno-rtti $(WARN)
CPPFLAGS += $(INCLUDES) -MD -MP
LDFLAGS  += -s

# End of File
