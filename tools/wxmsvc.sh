#!/bin/bash

#
# Configurable Parameters
#

# wxWidgets features to enable
ENABLE='GUI WCHAR_T THREADS FILE TEXTBUFFER CONFIG CONFIG_NATIVE
	CONTROLS BUTTON CHECKBOX GAUGE STATBOX STATTEXT TEXTCTRL TREECTRL
	STATUSBAR NATIVE_STATUSBAR NOTEBOOK IMAGLIST MENUS SPLITTER
	COMMON_DIALOGS DIRDLG FILEDLG MSGDLG PROGRESSDLG
	IPC IMAGE XPM ICO_CUR UXTHEME UXTHEME_AUTO'

# Dependencies for the above features
ENABLE="${ENABLE} DYNLIB_CLASS TEXTFILE RADIOBTN CHOICE TIMER"
ENABLE="${ENABLE} DATAOBJ DRAGIMAGE OLE WXDIB"

# wxWidgets build configuration
CONFIG=(
    'CFLAGS=/Ox'
    'CXXFLAGS=/Ox'
    'BUILD=release'
    'SHARED=0'
    'RUNTIME_LIBS=static'
    'VENDOR=dicomsel'
)


#
# Arguments Parsing
#

if [ $# -ne 1 ]; then
    echo "Syntax: $0 <wxWidgets source tree root>" 1>&2
    exit 1
fi

WXROOT="${1}"

if [ ! -d "${WXROOT}" ]; then
    echo "Error: '${WXROOT}' isn't a directory or doesn't exist." 1>&2
    exit 2
fi


#
# Update 'setup.h'
#

TOKCHAR='[0-9A-Za-z_]' # There shouldn't be 0-9 as first char, but don't care
BEFORE='^\([ \t]*#define[ \t]\+'
AFTER='[ \t]\+\)[01]\(\([ \t]\+.*\)\?\r\?\)$'
DEFAULT_DISABLED="wxUSE_${TOKCHAR}*\\|WXWIN_COMPATIBILITY_[0-9]_[0-9]"

expr=
for feature in ${ENABLE}; do
    expr="${expr}\\|${feature}"
done

sed -e "s/${BEFORE}\\(${DEFAULT_DISABLED}\\)${AFTER}/\\10\\3/" \
    -e "s/${BEFORE}wxUSE_\\(${expr:2}\\)${AFTER}/\\11\\3/" \
    -i.bak "${WXROOT}/include/wx/msw/setup.h"


#
# Update 'config.vc'
#

BEFORE='^\([ \t]*USE_'
AFTER='[ \t]*=[ \t]*\)[01]\(\([ \t]+.*\)\?\r\?\)$'

expr=
for feature in ${ENABLE}; do
    expr="${expr}\\|${feature}"
done

args="-e 's/${BEFORE}${TOKCHAR}*${AFTER}/\\10\\2/'"
args="${args} -e 's/${BEFORE}\\(${expr:2}\\)${AFTER}/\\11\\3/'"

for index in ${!CONFIG[*]}; do
    args="${args} -e 's%^\\(${CONFIG[$index]/=*/}\\)[ \\t]*=.*$"
    args="${args}%\1 = ${CONFIG[$index]/*=/}%'"
done

eval "sed ${args} -i.bak '${WXROOT}/build/msw/config.vc'"


#
# Bugfix
#

sed -e 's%\(#include[ \t]\+\)"wx/dynload\.h"%\1"wx/dynlib.h"%' -i.bak \
    "${WXROOT}/include/wx/msw/uxtheme.h"


#
# Build wxWidgets using MSVC in Wine
#

cd "${WXROOT}/build/msw"
exec wine nmake /NOLOGO /f makefile.vc

# End of File
