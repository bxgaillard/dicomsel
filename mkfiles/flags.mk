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


# Programs
CC     = $(PREFIX)gcc$(HOST_EXESUFFIX)
CXX    = $(PREFIX)g++$(HOST_EXESUFFIX)
STRIP  = $(PREFIX)strip$(HOST_EXESUFFIX)
RC     = $(PREFIX)windres$(HOST_EXESUFFIX)
AR     = $(PREFIX)ar$(HOST_EXESUFFIX)
RANLIB = $(PREFIX)ranlib$(HOST_EXESUFFIX)
RM     = rm$(HOST_EXESUFFIX) -f
CP     = cp$(HOST_EXESUFFIX) -fp
MV     = mv$(HOST_EXESUFFIX) -f

# Compiling Flags
ifneq ($(TARGET_OS),Windows)
    CFLAGS   ?= -O2 -fomit-frame-pointer -pipe
else
    CFLAGS   ?= -march=i386 -mtune=i686 -O2 -fomit-frame-pointer -pipe
    CPPFLAGS += -DLITTLE_ENDIAN=1234 -DBIG_ENDIAN=4321 -DBYTE_ORDER=1234
endif
CXXFLAGS ?= $(CFLAGS)
CXXFLAGS := $(CXXFLAGS)

# Custom Additions to Flags
WARN      = -Wall -Wextra
override CFLAGS   += $(WARN)
override CXXFLAGS += -fno-rtti $(WARN) -MD -MP
override CPPFLAGS += $(INCLUDES)

# Remove unsupported dependencies flags if compiling for several architectures
NUMARCH := $(words $(filter -arch,$(CXXFLAGS)))
ifneq ($(subst 1,0,$(NUMARCH)),0)
    override CXXFLAGS := $(filter-out -M%,$(CXXFLAGS))
endif

# End of File
