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


##############################################################################
#
# Operating Systems and Directories
#
#

# Detection of Supreme Evil ;)
ifneq ($(COMSPEC),)
    # COMSPEC is defined as the command line interpreter name under DOS and
    # then Windows 95, 98 and ME
    HOST_OS = Windows
endif
ifeq ($(HOST_OS),Windows_NT)
    # OS is defined as "Windows_NT" under Windows NT, 2000 and XP
    HOST_OS = Windows
endif

# The Really Good Ones
ifneq ($(HOST_OS),Windows)
    # If it isn't Windows, it's Unix, isn't it?
    HOST_OS = Unix
endif

# Determination of the Target OS
ifeq ($(TARGET_OS),unix)
    TARGET_OS = Unix
else
    ifeq ($(TARGET_OS),windows)
	TARGET_OS = Windows
    else
	ifeq ($(TARGET_OS),win)
	    TARGET_OS = Windows
	else
	    ifneq ($(TARGET_OS),Unix)
		ifneq ($(TARGET_OS),Windows)
		    TARGET_OS = $(HOST_OS)
		endif
	    endif
	endif
    endif
endif

# Programs Prefix
ifeq ($(HOST_OS).$(TARGET_OS),Unix.Windows)
    PREFIX ?= i386-mingw32msvc-
else
    # NOT SUPPORTED!
    ifeq ($(HOST_OS).$(TARGET_OS),Windows.Unix)
	PREFIX ?= i686-pc-linux-gnu-
    endif
endif

# Directory Separator
ifneq ($(HOST_OS),Windows)
    DIRSEP = /
else
    DIRSEP = /
    #DIRSEP = $(strip \)
endif

# Suffixes
ifneq ($(HOST_OS),Windows)
    HOST_EXESUFFIX =
else
    HOST_EXESUFFIX = .exe
endif
ifneq ($(TARGET_OS),Windows)
    EXESUFFIX =
else
    EXESUFFIX = .exe
endif

# Directories
ifneq ($(TOPDIR),)
    TOPDIR := $(TOPDIR)$(DIRSEP)
endif
MKFDIR = $(TOPDIR)mkfiles


##############################################################################
#
# Files
#
#

# Makefiles
MKFNAME = GNUmakefile
MKRULES = $(MKFDIR)$(DIRSEP)rules.mk $(MKFDIR)$(DIRSEP)flags.mk
MKFILE  = $(MKRULES) $(MKFNAME)

# Subdirectories
SUBDIRS := $(sort $(SUBDIRS) $(patsubst %$(DIRSEP)$(MKFNAME),%, \
					$(wildcard *$(DIRSEP)$(MKFNAME))))

# Source Files
SRC := $(SRC)
ifeq ($(SRC),)
    SRC := $(wildcard *.cpp)
endif

# Resource Files (under Windows)
RCFILES := $(sort $(RCFILES))
ifeq ($(RCFILES),)
    RCFILES := $(wildcard *.rc)
endif
ifneq ($(TARGET_OS),Windows)
    RCOBJ   := $(patsubst %.rc,%.o,$(RCFILES))
    RCFILES  =
else
    RCOBJ =
endif

# Object Files
OBJ := $(sort $(patsubst %.cpp,%.o,$(SRC)) $(patsubst %.rc,%.o,$(RCFILES)))

# Dependency Files
DEP := $(sort $(patsubst %.cpp,%.d,$(SRC)))

# Binaries and/or libraries
BINLIB = $(filter %.a,$(BIN)) \
	 $(patsubst %,%$(EXESUFFIX),$(filter-out %.a,$(BIN)))


##############################################################################
#
# Programs and Flags
#
#

# Programs and Flags Definitions
include $(MKFDIR)$(DIRSEP)flags.mk


##############################################################################
#
# Rules
#
#

# Pseudo-Targets
.PHONY: default final debug all unix windows win cleantmp clean $(SUBDIRS)
.SUFFIXES:
.SUFFIXES: .cpp .rc .o $(EXESUFFIX)
ifeq ($(NOSILENT),)
    .SILENT:
endif

# Generic Rules
%.o: %.cpp $(MKFILE)
	@echo Compiling $(CWD)$<...
	$(strip $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@)

%.o: %.rc $(MKFILE)
	@echo Compiling $(CWD)$<...
	$(strip $(RC) $(DEFINES) $(INCLUDES) -i $< -o $@)

# Phony Targets
ifeq ($(DEBUG),)
    default: final
else
    default: debug
endif

final: TARGET    = final
final: CPPFLAGS += -DNDEBUG
final: all

debug: TARGET    = debug
debug: CFLAGS   := -g $(filter-out -O%,$(CFLAGS))
debug: CXXFLAGS := -g $(filter-out -O%,$(CXXFLAGS))
debug: STRIP     =
#debug: CPPFLAGS += -DDEBUG -D_DEBUG
debug: LDFLAGS  := $(filter-out -s,$(LDFLAGS))
debug: all

all: $(SUBDIRS) $(BINLIB)

# Cross-Compilation
ifneq ($(HOST_OS),Windows)
    unix: default
    windows:
	@echo "++" Cross-compiling for Windows under Unix
	$(strip $(MAKE) TARGET_OS=Windows)
else
    unix:
	@echo Error: cannot cross-compile Unix programs under Windows.
	@exit 1
    windows: default
endif
win: windows

# Subdirectories
ifneq ($(SUBDIRS),)
    $(SUBDIRS): $(MKRULES)
	@echo "**" Entering directory $(CWD)$@...
	$(strip $(MAKE) -C $@ CWD=$(CWD)$@$(DIRSEP) TARGET_OS=$(TARGET_OS) \
		$(TARGET))
	@echo "**" Leaving directory $(CWD)$@...
endif

# Libraries
ifneq ($(DEPLIB),)
    $(DEPLIB): DIR = $(patsubst %$(DIRSEP),%,$(dir $@))
    $(DEPLIB): $(MKRULES)
	@echo "**" Entering directory $(DIR)...
	$(strip $(MAKE) -C $(DIR) CWD=$(CWD)$(DIR)$(DIRSEP) \
		TARGET_OS=$(TARGET_OS) $(TARGET))
	@echo "**" Leaving directory $(DIR)...
endif

# Main Binary
$(BINLIB): $(DEPLIB) $(OBJ) $(MKFILE)
    ifeq ($(patsubst %.a,,$(BIN)),$(BIN))
	@echo Linking $(CWD)$@...
	$(strip $(CXX) $(LDFLAGS) $(filter-out $(MKFILE) $(DEPLIB),$^) \
		$(LIBS) -o $@)
	$(if $(STRIP),@echo Stripping $(CWD)$@...,)
	$(if $(STRIP),$(strip $(STRIP) $@),)
	$(if $(subst @echo,,$(REZ)),@echo Adding resources to $(CWD)$@...,)
	$(if $(subst @echo,,$(REZ)),$(strip $(REZ) $@),)
    else
	@echo Creating $(CWD)$@...
	$(strip $(AR) -rcu $@ $(filter-out $(MKFILE),$^))
	$(strip $(RANLIB) $@)
    endif

# Temporary Files Removing
cleantmp: TARGET = cleantmp
cleantmp: $(SUBDIRS)
    ifneq ($(strip $(OBJ) $(RCOBJ) $(DEP) $(filter %.a,$(BIN))),)
	@echo Removing temporary files...
	$(strip $(RM) $(OBJ) $(RCOBJ) $(DEP) $(filter %.a,$(BIN)))
    endif

# Directory Cleaning
clean: TARGET = clean
clean: $(SUBDIRS)
    ifneq ($(strip $(OBJ) $(RCOBJ) $(DEP) $(BIN)),)
	@echo Cleaning up directory...
	$(strip $(RM) $(OBJ) $(RCOBJ) $(DEP) $(BIN) \
		$(patsubst %,%.exe,$(filter-out %.a,$(BIN))))
    endif


##############################################################################
#
# Dependencies
#
#

ifneq ($(DEP),)
    -include $(DEP)
endif

# End of File
