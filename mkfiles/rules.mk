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
# Programs, Flags and Directories
#
#

# Detection of Supreme Evil ;)
ifneq ($(COMSPEC),)
    # COMSPEC is defined as the command line interpreter name under DOS and
    # so Windows 95, 98 and ME
    OS = Windows
endif
ifeq ($(OS),Windows_NT)
    # OS is defined as "Windows_NT" under Windows NT, 2000 and XP
    OS = Windows
endif

ifneq ($(OS),Windows)
    DIRSEP = /
else
    DIRSEP = /
    #DIRSEP = $(strip \)
endif
ifneq ($(TOPDIR),)
    TOPDIR := $(TOPDIR)$(DIRSEP)
endif
MKFDIR = $(TOPDIR)mkfiles

include $(MKFDIR)$(DIRSEP)flags.mk


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

# Object Files
OBJ := $(sort $(patsubst %.cpp,%.o,$(SRC)) $(patsubst %.rc,%.o,$(RCFILES)))

# Dependency files
DEP := $(sort $(patsubst %.cpp,%.d,$(SRC)))


##############################################################################
#
# Rules
#
#

# Pseudo-Targets
.PHONY: default final debug all cleantmp clean $(SUBDIRS)
.SUFFIXES:
.SUFFIXES: .cpp .rc .o $(EXESUFFIX)
ifeq ($(NOSILENT),)
    .SILENT:
endif

# Generic Rules
%.o: %.cpp $(MKFILE)
	echo Compiling $(CWD)$<...
	$(strip $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@)

%.o: %.rc $(MKFILE)
	echo Compiling $(CWD)$<...
	$(strip $(RC) $(DEFINES) $(INCLUDES) -i $< -o $@)

# Phony Targets
default: final

final: CPPFLAGS += -DNDEBUG
final: all

debug: CPPFLAGS += -DDEBUG -D_DEBUG
debug: all

all: TARGET = all
all: $(SUBDIRS) $(BIN)

# Subdirectories
ifneq ($(SUBDIRS),)
    $(SUBDIRS):
	echo "**" Entering directory $(CWD)$@...
	$(strip $(MAKE) -C $@ CWD=$(CWD)$@$(DIRSEP) $(TARGET))
	echo "**" Leaving directory $(CWD)$@...
endif

# Libraries
ifneq ($(DEPLIB),)
    $(DEPLIB): DIR = $(patsubst %$(DIRSEP),%,$(dir $@))
    $(DEPLIB): $(MKRULES)
	echo "**" Entering directory $(DIR)...
	$(strip $(MAKE) -C $(DIR) CWD=$(CWD)$(DIR)$(DIRSEP) $(TARGET))
	echo "**" Leaving directory $(DIR)...
endif

# Main Binary
$(BIN): $(OBJ) $(MKFILE)
    ifeq ($(patsubst %.a,,$(BIN)),$(BIN))
	echo Linking $(CWD)$@...
	$(strip $(CXX) $(LDFLAGS) $(filter-out $(MKFILE),$^) $(LIBS) -o $@)
    else
	echo Creating $(CWD)$@...
	$(strip $(AR) -rcu $@ $(filter-out $(MKFILE),$^))
	$(strip $(RANLIB) $@)
    endif

# Temporary Files Removing
cleantmp: TARGET = cleantmp
cleantmp: $(SUBDIRS)
    ifneq ($(strip $(OBJ) $(DEP)),)
	echo Removing temporary files...
	$(strip $(RM) $(OBJ) $(DEP))
    endif

# Directory Cleaning
clean: TARGET = clean
clean: $(SUBDIRS)
    ifneq ($(strip $(OBJ) $(DEP) $(BIN)),)
	echo Cleaning up directory...
	$(strip $(RM) $(OBJ) $(DEP) $(BIN))
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
