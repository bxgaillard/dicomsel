dnl --------------------------------------------------------------------------
dnl
dnl DicomSel: DICOM Image Selector
dnl
dnl Copyright (C) 2005, 2006 IRCAD
dnl Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
dnl
dnl --------------------------------------------------------------------------
dnl
dnl        File: configure.ac
dnl
dnl Description: Autoconf Script
dnl
dnl --------------------------------------------------------------------------
dnl
dnl This program is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU General Public License as published by the Free
dnl Software Foundation; either version 2 of the License, or (at your option)
dnl any later version.
dnl
dnl This program is distributed in the hope that it will be useful, but WITHOUT
dnl ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
dnl FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
dnl more details.
dnl
dnl You should have received a copy of the GNU General Public License along
dnl with this program; if not, write to the Free Software Foundation, Inc., 59
dnl Temple Place - Suite 330, Boston, MA 02111-1307, USA.
dnl
dnl --------------------------------------------------------------------------


dnl Process this file with autoconf to produce a configure script.

dnl General settings
AC_PREREQ(2.52g)
AC_INIT([DicomSel], [1.9.0], [b.gaillard@powercode.net], [dicomsel])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR([autotools])
AM_INIT_AUTOMAKE([1.7 no-define dist-bzip2 -Wall])

dnl Additional definitions
AC_SUBST([PACKAGE_YEARS], ["2005, 2006"])
AC_DEFINE_UNQUOTED([PACKAGE_YEARS], ["$PACKAGE_YEARS"],
		   [The years associated to the copyright notice.])
AC_SUBST([PACKAGE_OWNER], ["IRCAD"])
AC_DEFINE_UNQUOTED([PACKAGE_OWNER], ["$PACKAGE_OWNER"],
		   [The package owner (copyright holder).])
AC_SUBST([PACKAGE_AUTHORS],
	 ["Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz"])
AC_DEFINE_UNQUOTED([PACKAGE_AUTHORS], ["$PACKAGE_AUTHORS"],
		   [The package authors.])
AC_SUBST([PACKAGE_AUTHORS_UNICODE],
	 ["Benjamin Gaillard, Marc-Aur\\u00E8le M\\u00F6rk, Guillaume Spitz"])
AC_DEFINE_UNQUOTED([PACKAGE_AUTHORS_UNICODE], ["$PACKAGE_AUTHORS_UNICODE"],
		   [The package authors.])

dnl Definitions for other packages
AC_DEFUN([DCMTK_AC_VERSION], [3.5.4])
AC_SUBST([DCMTK_VERSION], DCMTK_AC_VERSION)
AC_DEFUN([DCMTK_AC_DIR], [3rdparty/dcmtk-]DCMTK_AC_VERSION)
AC_SUBST([DCMTK_DIR], DCMTK_AC_DIR)
AC_DEFINE_UNQUOTED([DCMTK_VERSION], ["$DCMTK_VERSION"],
		   [The bundled DCMTK version.])

dnl Checks for programs
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_RANLIB
AC_CHECK_TOOL([RC], [windres])
AM_CONDITIONAL([HAVE_RC], [test "x$RC" != x])

dnl Checks for standard library, headers and functions
AC_HEADER_STDC
AC_HEADER_DIRENT
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_FUNC_STRTOD
AC_CHECK_FUNCS([strtol setlocale])
AC_STRUCT_TM

dnl Checks for libraries
AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG([2.6.0], [wxWin=1], [wxWin=0], [core,net,base])
if test "x$wxWin" != x1; then
    AC_MSG_ERROR([
	wxWidgets must be installed on your system
	but wx-config script couldn't be found.

	Please check that wx-config is in path, the directory
	where wxWidgets libraries are installed (returned by
	'wx-config --libs' command) is in LD_LIBRARY_PATH or
	equivalent variable and wxWidgets version is 2.3.4 or above.
    ])
fi

dnl Enable GCC/G++ warnings
if test "x$GCC" = xyes; then
    CFLAGS="-ansi -pedantic -Wall -W $CFLAGS"
fi
if test "x$GXX" = xyes; then
    CXXFLAGS="-Wall -W $CXXFLAGS"
fi

dnl Generated files
AC_CONFIG_FILES([
    Makefile
    libdicom/Makefile
    libdicom/src/Makefile
    src/Makefile
    src/Info.plist
    rfsample/Makefile
    rfsample/Info.plist
    doc/Makefile
    head/info.txt
    msvc/config.h
])

dnl Other packages configuration
AC_CONFIG_SUBDIRS(DCMTK_AC_DIR)

dnl Output configure files
AC_OUTPUT

dnl End of File
