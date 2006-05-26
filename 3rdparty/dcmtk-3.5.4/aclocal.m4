# generated automatically by aclocal 1.9.6 -*- Autoconf -*-

# Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004,
# 2005  Free Software Foundation, Inc.
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# Copyright (C) 2002, 2003, 2005  Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_AUTOMAKE_VERSION(VERSION)
# ----------------------------
# Automake X.Y traces this macro to ensure aclocal.m4 has been
# generated from the m4 files accompanying Automake X.Y.
AC_DEFUN([AM_AUTOMAKE_VERSION], [am__api_version="1.9"])

# AM_SET_CURRENT_AUTOMAKE_VERSION
# -------------------------------
# Call AM_AUTOMAKE_VERSION so it can be traced.
# This function is AC_REQUIREd by AC_INIT_AUTOMAKE.
AC_DEFUN([AM_SET_CURRENT_AUTOMAKE_VERSION],
	 [AM_AUTOMAKE_VERSION([1.9.6])])

# AM_AUX_DIR_EXPAND                                         -*- Autoconf -*-

# Copyright (C) 2001, 2003, 2005  Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# For projects using AC_CONFIG_AUX_DIR([foo]), Autoconf sets
# $ac_aux_dir to `$srcdir/foo'.  In other projects, it is set to
# `$srcdir', `$srcdir/..', or `$srcdir/../..'.
#
# Of course, Automake must honor this variable whenever it calls a
# tool from the auxiliary directory.  The problem is that $srcdir (and
# therefore $ac_aux_dir as well) can be either absolute or relative,
# depending on how configure is run.  This is pretty annoying, since
# it makes $ac_aux_dir quite unusable in subdirectories: in the top
# source directory, any form will work fine, but in subdirectories a
# relative path needs to be adjusted first.
#
# $ac_aux_dir/missing
#    fails when called from a subdirectory if $ac_aux_dir is relative
# $top_srcdir/$ac_aux_dir/missing
#    fails if $ac_aux_dir is absolute,
#    fails when called from a subdirectory in a VPATH build with
#          a relative $ac_aux_dir
#
# The reason of the latter failure is that $top_srcdir and $ac_aux_dir
# are both prefixed by $srcdir.  In an in-source build this is usually
# harmless because $srcdir is `.', but things will broke when you
# start a VPATH build or use an absolute $srcdir.
#
# So we could use something similar to $top_srcdir/$ac_aux_dir/missing,
# iff we strip the leading $srcdir from $ac_aux_dir.  That would be:
#   am_aux_dir='\$(top_srcdir)/'`expr "$ac_aux_dir" : "$srcdir//*\(.*\)"`
# and then we would define $MISSING as
#   MISSING="\${SHELL} $am_aux_dir/missing"
# This will work as long as MISSING is not called from configure, because
# unfortunately $(top_srcdir) has no meaning in configure.
# However there are other variables, like CC, which are often used in
# configure, and could therefore not use this "fixed" $ac_aux_dir.
#
# Another solution, used here, is to always expand $ac_aux_dir to an
# absolute PATH.  The drawback is that using absolute paths prevent a
# configured tree to be moved without reconfiguration.

AC_DEFUN([AM_AUX_DIR_EXPAND],
[dnl Rely on autoconf to set up CDPATH properly.
AC_PREREQ([2.50])dnl
# expand $ac_aux_dir to an absolute path
am_aux_dir=`cd $ac_aux_dir && pwd`
])

# AM_CONDITIONAL                                            -*- Autoconf -*-

# Copyright (C) 1997, 2000, 2001, 2003, 2004, 2005
# Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 7

# AM_CONDITIONAL(NAME, SHELL-CONDITION)
# -------------------------------------
# Define a conditional.
AC_DEFUN([AM_CONDITIONAL],
[AC_PREREQ(2.52)dnl
 ifelse([$1], [TRUE],  [AC_FATAL([$0: invalid condition: $1])],
	[$1], [FALSE], [AC_FATAL([$0: invalid condition: $1])])dnl
AC_SUBST([$1_TRUE])
AC_SUBST([$1_FALSE])
if $2; then
  $1_TRUE=
  $1_FALSE='#'
else
  $1_TRUE='#'
  $1_FALSE=
fi
AC_CONFIG_COMMANDS_PRE(
[if test -z "${$1_TRUE}" && test -z "${$1_FALSE}"; then
  AC_MSG_ERROR([[conditional "$1" was never defined.
Usually this means the macro was only invoked conditionally.]])
fi])])


# Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005
# Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 8

# There are a few dirty hacks below to avoid letting `AC_PROG_CC' be
# written in clear, in which case automake, when reading aclocal.m4,
# will think it sees a *use*, and therefore will trigger all it's
# C support machinery.  Also note that it means that autoscan, seeing
# CC etc. in the Makefile, will ask for an AC_PROG_CC use...


# _AM_DEPENDENCIES(NAME)
# ----------------------
# See how the compiler implements dependency checking.
# NAME is "CC", "CXX", "GCJ", or "OBJC".
# We try a few techniques and use that to set a single cache variable.
#
# We don't AC_REQUIRE the corresponding AC_PROG_CC since the latter was
# modified to invoke _AM_DEPENDENCIES(CC); we would have a circular
# dependency, and given that the user is not expected to run this macro,
# just rely on AC_PROG_CC.
AC_DEFUN([_AM_DEPENDENCIES],
[AC_REQUIRE([AM_SET_DEPDIR])dnl
AC_REQUIRE([AM_OUTPUT_DEPENDENCY_COMMANDS])dnl
AC_REQUIRE([AM_MAKE_INCLUDE])dnl
AC_REQUIRE([AM_DEP_TRACK])dnl

ifelse([$1], CC,   [depcc="$CC"   am_compiler_list=],
       [$1], CXX,  [depcc="$CXX"  am_compiler_list=],
       [$1], OBJC, [depcc="$OBJC" am_compiler_list='gcc3 gcc'],
       [$1], GCJ,  [depcc="$GCJ"  am_compiler_list='gcc3 gcc'],
                   [depcc="$$1"   am_compiler_list=])

AC_CACHE_CHECK([dependency style of $depcc],
               [am_cv_$1_dependencies_compiler_type],
[if test -z "$AMDEP_TRUE" && test -f "$am_depcomp"; then
  # We make a subdir and do the tests there.  Otherwise we can end up
  # making bogus files that we don't know about and never remove.  For
  # instance it was reported that on HP-UX the gcc test will end up
  # making a dummy file named `D' -- because `-MD' means `put the output
  # in D'.
  mkdir conftest.dir
  # Copy depcomp to subdir because otherwise we won't find it if we're
  # using a relative directory.
  cp "$am_depcomp" conftest.dir
  cd conftest.dir
  # We will build objects and dependencies in a subdirectory because
  # it helps to detect inapplicable dependency modes.  For instance
  # both Tru64's cc and ICC support -MD to output dependencies as a
  # side effect of compilation, but ICC will put the dependencies in
  # the current directory while Tru64 will put them in the object
  # directory.
  mkdir sub

  am_cv_$1_dependencies_compiler_type=none
  if test "$am_compiler_list" = ""; then
     am_compiler_list=`sed -n ['s/^#*\([a-zA-Z0-9]*\))$/\1/p'] < ./depcomp`
  fi
  for depmode in $am_compiler_list; do
    # Setup a source with many dependencies, because some compilers
    # like to wrap large dependency lists on column 80 (with \), and
    # we should not choose a depcomp mode which is confused by this.
    #
    # We need to recreate these files for each test, as the compiler may
    # overwrite some of them when testing with obscure command lines.
    # This happens at least with the AIX C compiler.
    : > sub/conftest.c
    for i in 1 2 3 4 5 6; do
      echo '#include "conftst'$i'.h"' >> sub/conftest.c
      # Using `: > sub/conftst$i.h' creates only sub/conftst1.h with
      # Solaris 8's {/usr,}/bin/sh.
      touch sub/conftst$i.h
    done
    echo "${am__include} ${am__quote}sub/conftest.Po${am__quote}" > confmf

    case $depmode in
    nosideeffect)
      # after this tag, mechanisms are not by side-effect, so they'll
      # only be used when explicitly requested
      if test "x$enable_dependency_tracking" = xyes; then
	continue
      else
	break
      fi
      ;;
    none) break ;;
    esac
    # We check with `-c' and `-o' for the sake of the "dashmstdout"
    # mode.  It turns out that the SunPro C++ compiler does not properly
    # handle `-M -o', and we need to detect this.
    if depmode=$depmode \
       source=sub/conftest.c object=sub/conftest.${OBJEXT-o} \
       depfile=sub/conftest.Po tmpdepfile=sub/conftest.TPo \
       $SHELL ./depcomp $depcc -c -o sub/conftest.${OBJEXT-o} sub/conftest.c \
         >/dev/null 2>conftest.err &&
       grep sub/conftst6.h sub/conftest.Po > /dev/null 2>&1 &&
       grep sub/conftest.${OBJEXT-o} sub/conftest.Po > /dev/null 2>&1 &&
       ${MAKE-make} -s -f confmf > /dev/null 2>&1; then
      # icc doesn't choke on unknown options, it will just issue warnings
      # or remarks (even with -Werror).  So we grep stderr for any message
      # that says an option was ignored or not supported.
      # When given -MP, icc 7.0 and 7.1 complain thusly:
      #   icc: Command line warning: ignoring option '-M'; no argument required
      # The diagnosis changed in icc 8.0:
      #   icc: Command line remark: option '-MP' not supported
      if (grep 'ignoring option' conftest.err ||
          grep 'not supported' conftest.err) >/dev/null 2>&1; then :; else
        am_cv_$1_dependencies_compiler_type=$depmode
        break
      fi
    fi
  done

  cd ..
  rm -rf conftest.dir
else
  am_cv_$1_dependencies_compiler_type=none
fi
])
AC_SUBST([$1DEPMODE], [depmode=$am_cv_$1_dependencies_compiler_type])
AM_CONDITIONAL([am__fastdep$1], [
  test "x$enable_dependency_tracking" != xno \
  && test "$am_cv_$1_dependencies_compiler_type" = gcc3])
])


# AM_SET_DEPDIR
# -------------
# Choose a directory name for dependency files.
# This macro is AC_REQUIREd in _AM_DEPENDENCIES
AC_DEFUN([AM_SET_DEPDIR],
[AC_REQUIRE([AM_SET_LEADING_DOT])dnl
AC_SUBST([DEPDIR], ["${am__leading_dot}deps"])dnl
])


# AM_DEP_TRACK
# ------------
AC_DEFUN([AM_DEP_TRACK],
[AC_ARG_ENABLE(dependency-tracking,
[  --disable-dependency-tracking  speeds up one-time build
  --enable-dependency-tracking   do not reject slow dependency extractors])
if test "x$enable_dependency_tracking" != xno; then
  am_depcomp="$ac_aux_dir/depcomp"
  AMDEPBACKSLASH='\'
fi
AM_CONDITIONAL([AMDEP], [test "x$enable_dependency_tracking" != xno])
AC_SUBST([AMDEPBACKSLASH])
])

# Generate code to set up dependency tracking.              -*- Autoconf -*-

# Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005
# Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

#serial 3

# _AM_OUTPUT_DEPENDENCY_COMMANDS
# ------------------------------
AC_DEFUN([_AM_OUTPUT_DEPENDENCY_COMMANDS],
[for mf in $CONFIG_FILES; do
  # Strip MF so we end up with the name of the file.
  mf=`echo "$mf" | sed -e 's/:.*$//'`
  # Check whether this is an Automake generated Makefile or not.
  # We used to match only the files named `Makefile.in', but
  # some people rename them; so instead we look at the file content.
  # Grep'ing the first line is not enough: some people post-process
  # each Makefile.in and add a new line on top of each file to say so.
  # So let's grep whole file.
  if grep '^#.*generated by automake' $mf > /dev/null 2>&1; then
    dirpart=`AS_DIRNAME("$mf")`
  else
    continue
  fi
  # Extract the definition of DEPDIR, am__include, and am__quote
  # from the Makefile without running `make'.
  DEPDIR=`sed -n 's/^DEPDIR = //p' < "$mf"`
  test -z "$DEPDIR" && continue
  am__include=`sed -n 's/^am__include = //p' < "$mf"`
  test -z "am__include" && continue
  am__quote=`sed -n 's/^am__quote = //p' < "$mf"`
  # When using ansi2knr, U may be empty or an underscore; expand it
  U=`sed -n 's/^U = //p' < "$mf"`
  # Find all dependency output files, they are included files with
  # $(DEPDIR) in their names.  We invoke sed twice because it is the
  # simplest approach to changing $(DEPDIR) to its actual value in the
  # expansion.
  for file in `sed -n "
    s/^$am__include $am__quote\(.*(DEPDIR).*\)$am__quote"'$/\1/p' <"$mf" | \
       sed -e 's/\$(DEPDIR)/'"$DEPDIR"'/g' -e 's/\$U/'"$U"'/g'`; do
    # Make sure the directory exists.
    test -f "$dirpart/$file" && continue
    fdir=`AS_DIRNAME(["$file"])`
    AS_MKDIR_P([$dirpart/$fdir])
    # echo "creating $dirpart/$file"
    echo '# dummy' > "$dirpart/$file"
  done
done
])# _AM_OUTPUT_DEPENDENCY_COMMANDS


# AM_OUTPUT_DEPENDENCY_COMMANDS
# -----------------------------
# This macro should only be invoked once -- use via AC_REQUIRE.
#
# This code is only required when automatic dependency tracking
# is enabled.  FIXME.  This creates each `.P' file that we will
# need in order to bootstrap the dependency handling code.
AC_DEFUN([AM_OUTPUT_DEPENDENCY_COMMANDS],
[AC_CONFIG_COMMANDS([depfiles],
     [test x"$AMDEP_TRUE" != x"" || _AM_OUTPUT_DEPENDENCY_COMMANDS],
     [AMDEP_TRUE="$AMDEP_TRUE" ac_aux_dir="$ac_aux_dir"])
])

# Do all the work for Automake.                             -*- Autoconf -*-

# Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005
# Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 12

# This macro actually does too much.  Some checks are only needed if
# your package does certain things.  But this isn't really a big deal.

# AM_INIT_AUTOMAKE(PACKAGE, VERSION, [NO-DEFINE])
# AM_INIT_AUTOMAKE([OPTIONS])
# -----------------------------------------------
# The call with PACKAGE and VERSION arguments is the old style
# call (pre autoconf-2.50), which is being phased out.  PACKAGE
# and VERSION should now be passed to AC_INIT and removed from
# the call to AM_INIT_AUTOMAKE.
# We support both call styles for the transition.  After
# the next Automake release, Autoconf can make the AC_INIT
# arguments mandatory, and then we can depend on a new Autoconf
# release and drop the old call support.
AC_DEFUN([AM_INIT_AUTOMAKE],
[AC_PREREQ([2.58])dnl
dnl Autoconf wants to disallow AM_ names.  We explicitly allow
dnl the ones we care about.
m4_pattern_allow([^AM_[A-Z]+FLAGS$])dnl
AC_REQUIRE([AM_SET_CURRENT_AUTOMAKE_VERSION])dnl
AC_REQUIRE([AC_PROG_INSTALL])dnl
# test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" &&
   test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi

# test whether we have cygpath
if test -z "$CYGPATH_W"; then
  if (cygpath --version) >/dev/null 2>/dev/null; then
    CYGPATH_W='cygpath -w'
  else
    CYGPATH_W=echo
  fi
fi
AC_SUBST([CYGPATH_W])

# Define the identity of the package.
dnl Distinguish between old-style and new-style calls.
m4_ifval([$2],
[m4_ifval([$3], [_AM_SET_OPTION([no-define])])dnl
 AC_SUBST([PACKAGE], [$1])dnl
 AC_SUBST([VERSION], [$2])],
[_AM_SET_OPTIONS([$1])dnl
 AC_SUBST([PACKAGE], ['AC_PACKAGE_TARNAME'])dnl
 AC_SUBST([VERSION], ['AC_PACKAGE_VERSION'])])dnl

_AM_IF_OPTION([no-define],,
[AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
 AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package])])dnl

# Some tools Automake needs.
AC_REQUIRE([AM_SANITY_CHECK])dnl
AC_REQUIRE([AC_ARG_PROGRAM])dnl
AM_MISSING_PROG(ACLOCAL, aclocal-${am__api_version})
AM_MISSING_PROG(AUTOCONF, autoconf)
AM_MISSING_PROG(AUTOMAKE, automake-${am__api_version})
AM_MISSING_PROG(AUTOHEADER, autoheader)
AM_MISSING_PROG(MAKEINFO, makeinfo)
AM_PROG_INSTALL_SH
AM_PROG_INSTALL_STRIP
AC_REQUIRE([AM_PROG_MKDIR_P])dnl
# We need awk for the "check" target.  The system "awk" is bad on
# some platforms.
AC_REQUIRE([AC_PROG_AWK])dnl
AC_REQUIRE([AC_PROG_MAKE_SET])dnl
AC_REQUIRE([AM_SET_LEADING_DOT])dnl
_AM_IF_OPTION([tar-ustar], [_AM_PROG_TAR([ustar])],
              [_AM_IF_OPTION([tar-pax], [_AM_PROG_TAR([pax])],
	      		     [_AM_PROG_TAR([v7])])])
_AM_IF_OPTION([no-dependencies],,
[AC_PROVIDE_IFELSE([AC_PROG_CC],
                  [_AM_DEPENDENCIES(CC)],
                  [define([AC_PROG_CC],
                          defn([AC_PROG_CC])[_AM_DEPENDENCIES(CC)])])dnl
AC_PROVIDE_IFELSE([AC_PROG_CXX],
                  [_AM_DEPENDENCIES(CXX)],
                  [define([AC_PROG_CXX],
                          defn([AC_PROG_CXX])[_AM_DEPENDENCIES(CXX)])])dnl
])
])


# When config.status generates a header, we must update the stamp-h file.
# This file resides in the same directory as the config header
# that is generated.  The stamp files are numbered to have different names.

# Autoconf calls _AC_AM_CONFIG_HEADER_HOOK (when defined) in the
# loop where config.status creates the headers, so we can generate
# our stamp files there.
AC_DEFUN([_AC_AM_CONFIG_HEADER_HOOK],
[# Compute $1's index in $config_headers.
_am_stamp_count=1
for _am_header in $config_headers :; do
  case $_am_header in
    $1 | $1:* )
      break ;;
    * )
      _am_stamp_count=`expr $_am_stamp_count + 1` ;;
  esac
done
echo "timestamp for $1" >`AS_DIRNAME([$1])`/stamp-h[]$_am_stamp_count])

# Copyright (C) 2001, 2003, 2005  Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_PROG_INSTALL_SH
# ------------------
# Define $install_sh.
AC_DEFUN([AM_PROG_INSTALL_SH],
[AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
install_sh=${install_sh-"$am_aux_dir/install-sh"}
AC_SUBST(install_sh)])

# Copyright (C) 2003, 2005  Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 2

# Check whether the underlying file-system supports filenames
# with a leading dot.  For instance MS-DOS doesn't.
AC_DEFUN([AM_SET_LEADING_DOT],
[rm -rf .tst 2>/dev/null
mkdir .tst 2>/dev/null
if test -d .tst; then
  am__leading_dot=.
else
  am__leading_dot=_
fi
rmdir .tst 2>/dev/null
AC_SUBST([am__leading_dot])])

# Check to see how 'make' treats includes.	            -*- Autoconf -*-

# Copyright (C) 2001, 2002, 2003, 2005  Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 3

# AM_MAKE_INCLUDE()
# -----------------
# Check to see how make treats includes.
AC_DEFUN([AM_MAKE_INCLUDE],
[am_make=${MAKE-make}
cat > confinc << 'END'
am__doit:
	@echo done
.PHONY: am__doit
END
# If we don't find an include directive, just comment out the code.
AC_MSG_CHECKING([for style of include used by $am_make])
am__include="#"
am__quote=
_am_result=none
# First try GNU make style include.
echo "include confinc" > confmf
# We grep out `Entering directory' and `Leaving directory'
# messages which can occur if `w' ends up in MAKEFLAGS.
# In particular we don't look at `^make:' because GNU make might
# be invoked under some other name (usually "gmake"), in which
# case it prints its new name instead of `make'.
if test "`$am_make -s -f confmf 2> /dev/null | grep -v 'ing directory'`" = "done"; then
   am__include=include
   am__quote=
   _am_result=GNU
fi
# Now try BSD make style include.
if test "$am__include" = "#"; then
   echo '.include "confinc"' > confmf
   if test "`$am_make -s -f confmf 2> /dev/null`" = "done"; then
      am__include=.include
      am__quote="\""
      _am_result=BSD
   fi
fi
AC_SUBST([am__include])
AC_SUBST([am__quote])
AC_MSG_RESULT([$_am_result])
rm -f confinc confmf
])

# Copyright (C) 1999, 2000, 2001, 2003, 2005  Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 3

# AM_PROG_CC_C_O
# --------------
# Like AC_PROG_CC_C_O, but changed for automake.
AC_DEFUN([AM_PROG_CC_C_O],
[AC_REQUIRE([AC_PROG_CC_C_O])dnl
AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
# FIXME: we rely on the cache variable name because
# there is no other way.
set dummy $CC
ac_cc=`echo $[2] | sed ['s/[^a-zA-Z0-9_]/_/g;s/^[0-9]/_/']`
if eval "test \"`echo '$ac_cv_prog_cc_'${ac_cc}_c_o`\" != yes"; then
   # Losing compiler, so override with the script.
   # FIXME: It is wrong to rewrite CC.
   # But if we don't then we get into trouble of one sort or another.
   # A longer-term fix would be to have automake use am__CC in this case,
   # and then we could set am__CC="\$(top_srcdir)/compile \$(CC)"
   CC="$am_aux_dir/compile $CC"
fi
])

# Fake the existence of programs that GNU maintainers use.  -*- Autoconf -*-

# Copyright (C) 1997, 1999, 2000, 2001, 2003, 2005
# Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 4

# AM_MISSING_PROG(NAME, PROGRAM)
# ------------------------------
AC_DEFUN([AM_MISSING_PROG],
[AC_REQUIRE([AM_MISSING_HAS_RUN])
$1=${$1-"${am_missing_run}$2"}
AC_SUBST($1)])


# AM_MISSING_HAS_RUN
# ------------------
# Define MISSING if not defined so far and test if it supports --run.
# If it does, set am_missing_run to use it, otherwise, to nothing.
AC_DEFUN([AM_MISSING_HAS_RUN],
[AC_REQUIRE([AM_AUX_DIR_EXPAND])dnl
test x"${MISSING+set}" = xset || MISSING="\${SHELL} $am_aux_dir/missing"
# Use eval to expand $SHELL
if eval "$MISSING --run true"; then
  am_missing_run="$MISSING --run "
else
  am_missing_run=
  AC_MSG_WARN([`missing' script is too old or missing])
fi
])

# Copyright (C) 2003, 2004, 2005  Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_PROG_MKDIR_P
# ---------------
# Check whether `mkdir -p' is supported, fallback to mkinstalldirs otherwise.
#
# Automake 1.8 used `mkdir -m 0755 -p --' to ensure that directories
# created by `make install' are always world readable, even if the
# installer happens to have an overly restrictive umask (e.g. 077).
# This was a mistake.  There are at least two reasons why we must not
# use `-m 0755':
#   - it causes special bits like SGID to be ignored,
#   - it may be too restrictive (some setups expect 775 directories).
#
# Do not use -m 0755 and let people choose whatever they expect by
# setting umask.
#
# We cannot accept any implementation of `mkdir' that recognizes `-p'.
# Some implementations (such as Solaris 8's) are not thread-safe: if a
# parallel make tries to run `mkdir -p a/b' and `mkdir -p a/c'
# concurrently, both version can detect that a/ is missing, but only
# one can create it and the other will error out.  Consequently we
# restrict ourselves to GNU make (using the --version option ensures
# this.)
AC_DEFUN([AM_PROG_MKDIR_P],
[if mkdir -p --version . >/dev/null 2>&1 && test ! -d ./--version; then
  # We used to keeping the `.' as first argument, in order to
  # allow $(mkdir_p) to be used without argument.  As in
  #   $(mkdir_p) $(somedir)
  # where $(somedir) is conditionally defined.  However this is wrong
  # for two reasons:
  #  1. if the package is installed by a user who cannot write `.'
  #     make install will fail,
  #  2. the above comment should most certainly read
  #     $(mkdir_p) $(DESTDIR)$(somedir)
  #     so it does not work when $(somedir) is undefined and
  #     $(DESTDIR) is not.
  #  To support the latter case, we have to write
  #     test -z "$(somedir)" || $(mkdir_p) $(DESTDIR)$(somedir),
  #  so the `.' trick is pointless.
  mkdir_p='mkdir -p --'
else
  # On NextStep and OpenStep, the `mkdir' command does not
  # recognize any option.  It will interpret all options as
  # directories to create, and then abort because `.' already
  # exists.
  for d in ./-p ./--version;
  do
    test -d $d && rmdir $d
  done
  # $(mkinstalldirs) is defined by Automake if mkinstalldirs exists.
  if test -f "$ac_aux_dir/mkinstalldirs"; then
    mkdir_p='$(mkinstalldirs)'
  else
    mkdir_p='$(install_sh) -d'
  fi
fi
AC_SUBST([mkdir_p])])

# Helper functions for option handling.                     -*- Autoconf -*-

# Copyright (C) 2001, 2002, 2003, 2005  Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 3

# _AM_MANGLE_OPTION(NAME)
# -----------------------
AC_DEFUN([_AM_MANGLE_OPTION],
[[_AM_OPTION_]m4_bpatsubst($1, [[^a-zA-Z0-9_]], [_])])

# _AM_SET_OPTION(NAME)
# ------------------------------
# Set option NAME.  Presently that only means defining a flag for this option.
AC_DEFUN([_AM_SET_OPTION],
[m4_define(_AM_MANGLE_OPTION([$1]), 1)])

# _AM_SET_OPTIONS(OPTIONS)
# ----------------------------------
# OPTIONS is a space-separated list of Automake options.
AC_DEFUN([_AM_SET_OPTIONS],
[AC_FOREACH([_AM_Option], [$1], [_AM_SET_OPTION(_AM_Option)])])

# _AM_IF_OPTION(OPTION, IF-SET, [IF-NOT-SET])
# -------------------------------------------
# Execute IF-SET if OPTION is set, IF-NOT-SET otherwise.
AC_DEFUN([_AM_IF_OPTION],
[m4_ifset(_AM_MANGLE_OPTION([$1]), [$2], [$3])])

# Check to make sure that the build environment is sane.    -*- Autoconf -*-

# Copyright (C) 1996, 1997, 2000, 2001, 2003, 2005
# Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 4

# AM_SANITY_CHECK
# ---------------
AC_DEFUN([AM_SANITY_CHECK],
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftest.file
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftest.file 2> /dev/null`
   if test "$[*]" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftest.file`
   fi
   rm -f conftest.file
   if test "$[*]" != "X $srcdir/configure conftest.file" \
      && test "$[*]" != "X conftest.file $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "$[2]" = conftest.file
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
AC_MSG_RESULT(yes)])

# Copyright (C) 2001, 2003, 2005  Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# AM_PROG_INSTALL_STRIP
# ---------------------
# One issue with vendor `install' (even GNU) is that you can't
# specify the program used to strip binaries.  This is especially
# annoying in cross-compiling environments, where the build's strip
# is unlikely to handle the host's binaries.
# Fortunately install-sh will honor a STRIPPROG variable, so we
# always use install-sh in `make install-strip', and initialize
# STRIPPROG with the value of the STRIP variable (set by the user).
AC_DEFUN([AM_PROG_INSTALL_STRIP],
[AC_REQUIRE([AM_PROG_INSTALL_SH])dnl
# Installed binaries are usually stripped using `strip' when the user
# run `make install-strip'.  However `strip' might not be the right
# tool to use in cross-compilation environments, therefore Automake
# will honor the `STRIP' environment variable to overrule this program.
dnl Don't test for $cross_compiling = yes, because it might be `maybe'.
if test "$cross_compiling" != no; then
  AC_CHECK_TOOL([STRIP], [strip], :)
fi
INSTALL_STRIP_PROGRAM="\${SHELL} \$(install_sh) -c -s"
AC_SUBST([INSTALL_STRIP_PROGRAM])])

# Check how to create a tarball.                            -*- Autoconf -*-

# Copyright (C) 2004, 2005  Free Software Foundation, Inc.
#
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 2

# _AM_PROG_TAR(FORMAT)
# --------------------
# Check how to create a tarball in format FORMAT.
# FORMAT should be one of `v7', `ustar', or `pax'.
#
# Substitute a variable $(am__tar) that is a command
# writing to stdout a FORMAT-tarball containing the directory
# $tardir.
#     tardir=directory && $(am__tar) > result.tar
#
# Substitute a variable $(am__untar) that extract such
# a tarball read from stdin.
#     $(am__untar) < result.tar
AC_DEFUN([_AM_PROG_TAR],
[# Always define AMTAR for backward compatibility.
AM_MISSING_PROG([AMTAR], [tar])
m4_if([$1], [v7],
     [am__tar='${AMTAR} chof - "$$tardir"'; am__untar='${AMTAR} xf -'],
     [m4_case([$1], [ustar],, [pax],,
              [m4_fatal([Unknown tar format])])
AC_MSG_CHECKING([how to create a $1 tar archive])
# Loop over all known methods to create a tar archive until one works.
_am_tools='gnutar m4_if([$1], [ustar], [plaintar]) pax cpio none'
_am_tools=${am_cv_prog_tar_$1-$_am_tools}
# Do not fold the above two line into one, because Tru64 sh and
# Solaris sh will not grok spaces in the rhs of `-'.
for _am_tool in $_am_tools
do
  case $_am_tool in
  gnutar)
    for _am_tar in tar gnutar gtar;
    do
      AM_RUN_LOG([$_am_tar --version]) && break
    done
    am__tar="$_am_tar --format=m4_if([$1], [pax], [posix], [$1]) -chf - "'"$$tardir"'
    am__tar_="$_am_tar --format=m4_if([$1], [pax], [posix], [$1]) -chf - "'"$tardir"'
    am__untar="$_am_tar -xf -"
    ;;
  plaintar)
    # Must skip GNU tar: if it does not support --format= it doesn't create
    # ustar tarball either.
    (tar --version) >/dev/null 2>&1 && continue
    am__tar='tar chf - "$$tardir"'
    am__tar_='tar chf - "$tardir"'
    am__untar='tar xf -'
    ;;
  pax)
    am__tar='pax -L -x $1 -w "$$tardir"'
    am__tar_='pax -L -x $1 -w "$tardir"'
    am__untar='pax -r'
    ;;
  cpio)
    am__tar='find "$$tardir" -print | cpio -o -H $1 -L'
    am__tar_='find "$tardir" -print | cpio -o -H $1 -L'
    am__untar='cpio -i -H $1 -d'
    ;;
  none)
    am__tar=false
    am__tar_=false
    am__untar=false
    ;;
  esac

  # If the value was cached, stop now.  We just wanted to have am__tar
  # and am__untar set.
  test -n "${am_cv_prog_tar_$1}" && break

  # tar/untar a dummy directory, and stop if the command works
  rm -rf conftest.dir
  mkdir conftest.dir
  echo GrepMe > conftest.dir/file
  AM_RUN_LOG([tardir=conftest.dir && eval $am__tar_ >conftest.tar])
  rm -rf conftest.dir
  if test -s conftest.tar; then
    AM_RUN_LOG([$am__untar <conftest.tar])
    grep GrepMe conftest.dir/file >/dev/null 2>&1 && break
  fi
done
rm -rf conftest.dir

AC_CACHE_VAL([am_cv_prog_tar_$1], [am_cv_prog_tar_$1=$_am_tool])
AC_MSG_RESULT([$am_cv_prog_tar_$1])])
AC_SUBST([am__tar])
AC_SUBST([am__untar])
]) # _AM_PROG_TAR

dnl 
dnl Filename: aclocal.m4
dnl
dnl Purpose: additional M4 macros for GNU autoconf
dnl
dnl Authors: Andreas Barth, Marco Eichelberg
dnl
dnl Last Update:  $Author: meichel $
dnl Revision:     $Revision: 1.37 $
dnl Status:       $State: Exp $
dnl


dnl AC_CHECK_GXXLIB works like AC_CHECK_LIB, but for libg++.
dnl   This additional macro is necessary because the /bin/sh will
dnl   not allow you to define a variable "ac_cv_lib_g++" and
dnl   thus AC_CHECK_LIB will fail for libg++.
dnl   All symbols #defined by this macro replace G++ by GXX.

dnl AC_CHECK_GXXLIB
AC_DEFUN(AC_CHECK_GXXLIB,
[AC_MSG_CHECKING([for -lg++])
AC_CACHE_VAL(ac_cv_lib_gxx,
[ac_save_LIBS="$LIBS"
LIBS="-lg++ $LIBS"
AC_TRY_LINK(, [main()], eval "ac_cv_lib_gxx=yes", eval "ac_cv_lib_gxx=no")
LIBS="$ac_save_LIBS"
])dnl
if eval "test \"`echo '$ac_cv_lib_gxx'`\" = yes"; then
  AC_MSG_RESULT(yes)
  ifelse([$3], , 
[changequote(, )dnl
  ac_tr_lib=HAVE_LIBGXX
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_lib)
  LIBS="-lg++ $LIBS"
], [$3])
else
  AC_MSG_RESULT(no)
ifelse([$4], , , [$4
])dnl
fi
])

dnl AC_CHECK_PROTOTYPE checks if there is a prototype declaration
dnl   for the given function. If header file(s) are given as argument 2, they
dnl   are #included in the search. Otherwise only predefined functions will
dnl   be found.  The header files are only included in the search if they 
dnl   have already been found using the AC_CHECK_HEADERS(header) macro.  
dnl Note:
dnl   Since GNU autoheader does not support this macro, you must create entries
dnl   in your acconfig.h for each function which is tested.
dnl Examples:
dnl   in configure.in: 
dnl     AC_CHECK_PROTOTYPE(setsockopt, sys/types.h sys/socket.h)
dnl     AC_CHECK_PROTOTYPE(gethostid)
dnl   in acconfig.h:
dnl     #undef HAVE_PROTOTYPE_SETSOCKOPT
dnl     #undef HAVE_PROTOTYPE_GETHOSTID

dnl AC_CHECK_PROTOTYPE(FUNCTION, HEADER-FILE..., ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
AC_DEFUN(AC_CHECK_PROTOTYPE,
[AC_MSG_CHECKING([ifelse([$2], , [predefined prototype for $1], [prototype for $1 (in $2)])])
AH_TEMPLATE(AS_TR_CPP(HAVE_PROTOTYPE_$1), [Define if your system has a prototype for $1 in $2])
ifelse([$3], , :, [$3])

ifelse([$2], , [ac_includes=""
],
[ac_includes=""
for ac_header in $2
do
  ac_safe=`echo "$ac_header" | sed 'y%./+-%__p_%'`
  if eval "test \"`echo '$''{'ac_cv_header_$ac_safe'}'`\" = yes"; then
    ac_includes="$ac_includes 
#include<$ac_header>"
  fi
done])
tmp_save_1=`echo $1 | tr ' :' '__'`
AC_CACHE_VAL(ac_cv_prototype_$tmp_save_1,
[AC_TRY_COMPILE(
[#ifdef __cplusplus
extern "C" {
#endif
$ac_includes
#ifdef __cplusplus
}
#endif
typedef union { int member; } dummyStruct;
#ifdef __cplusplus
extern "C"
#endif
dummyStruct $1(dummyStruct);
]
, ,eval "ac_cv_prototype_$tmp_save_1=no", eval "ac_cv_prototype_$tmp_save_1=yes")])dnl
if eval "test \"`echo '$''{'ac_cv_prototype_$tmp_save_1'}'`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_prototype=HAVE_PROTOTYPE_`echo $tmp_save_1 | tr '[a-z]' '[A-Z]'`
changequote([, ])dnl
  AC_DEFINE_UNQUOTED([$ac_tr_prototype])
  ifelse([$3], , :, [$3])
else
  AC_MSG_RESULT(no)
ifelse([$4], , , [$4
])dnl
fi
])

dnl AC_CHECK_DECLARATION checks if a certain type is declared in the include files given as argument 2 or 3.
dnl   Files given as argument 2 are included extern "C" in C++ mode,
dnl   files given as argument 3 are included "as is".
dnl   Header files are only included in the search if they 
dnl   have already been found using the AC_CHECK_HEADERS(header) macro.  

dnl AC_CHECK_DECLARATION(FUNCTION, C-HEADER-FILE..., C++-HEADER-FILE..., ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
AC_DEFUN(AC_CHECK_DECLARATION,
[
AC_MSG_CHECKING([ifelse([$2 $3], , [predefined type $1], [ifelse([$2], , [declaration for $1 (in $3)], [ifelse([$3], , [declaration for $1 (in $2)], [declaration for $1 (in $2 $3)])])])])
AH_TEMPLATE(AS_TR_CPP(HAVE_DECLARATION_$1), [Define if your system has a declaration for $1 in $2 $3])
ifelse([$2], , [ac_includes=""
],
[ac_includes=""
for ac_header in $2
do
  ac_safe=`echo "$ac_header" | sed 'y%./+-%__p_%'`
  if eval "test \"`echo '$''{'ac_cv_header_$ac_safe'}'`\" = yes"; then
    ac_includes="$ac_includes 
#include<$ac_header>"
  fi
done
])
ifelse([$3], , [ac_cpp_includes=""
],
[ac_cpp_includes=""
for ac_header in $3
do
  ac_safe=`echo "$ac_header" | sed 'y%./+-%__p_%'`
  if eval "test \"`echo '$''{'ac_cv_header_$ac_safe'}'`\" = yes"; then
    ac_cpp_includes="$ac_cpp_includes 
#include<$ac_header>"
  fi
done
])
[ac_cv_declaration=`echo 'ac_cv_declaration_$1' | tr ' :' '__'`
]
AC_CACHE_VAL($ac_cv_declaration,
[AC_TRY_COMPILE(
[#ifdef __cplusplus
extern "C" {
#endif
$ac_includes
#ifdef __cplusplus
}
#endif
$ac_cpp_includes
$1 dummy;
]
, ,eval "$ac_cv_declaration=yes", eval "$ac_cv_declaration=no")])dnl
if eval "test \"\$$ac_cv_declaration\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_declaration=HAVE_DECLARATION_`echo $1 | tr ' :[a-z]' '__[A-Z]'`
changequote([, ])dnl
  AC_DEFINE_UNQUOTED([$ac_tr_declaration])
  ifelse([$4], , :, [$4])
else
  AC_MSG_RESULT(no)
ifelse([$5], , , [$5
])dnl
fi
unset ac_cv_declaration
])


dnl AC_CHECK_INTP_SELECT checks if the prototype for select()
dnl   specifies arguments 2-4 to be int* instead of struct fd_set *.
dnl   This is true for HP UX 9.x and causes C++ code to break.

dnl AC_CHECK_INTP_SELECT(HEADER-FILE..., ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
AC_DEFUN(AC_CHECK_INTP_SELECT,
[AC_MSG_CHECKING([ifelse([$1], , [if select() needs int* parameters], 
[if select() needs int* parameters (in $1)])])
AH_TEMPLATE([HAVE_INTP_SELECT], [Define if your system declares argument 2-4 of select()
   as int * instead of struct fd_set *])
ifelse([$1], , [ac_includes=""
],
[ac_includes=""
for ac_header in $1
do
  ac_safe=`echo "$ac_header" | sed 'y%./+-%__p_%'`
  if eval "test \"`echo '$''{'ac_cv_header_$ac_safe'}'`\" = yes"; then
    ac_includes="$ac_includes 
#include<$ac_header>"
  fi
done])
AC_CACHE_VAL(ac_cv_prototype_intp_select,
[AC_TRY_COMPILE(
[#ifdef __cplusplus
extern "C" {
#endif
$ac_includes
#ifdef __cplusplus
}
#endif
]
,
[
  int i;
  fd_set fds;
  i = select(1, &fds, &fds, &fds, NULL);
],
eval "ac_cv_prototype_intp_select=no", 
[AC_TRY_COMPILE(
[#ifdef __cplusplus
extern "C" {
#endif
$ac_includes
#ifdef __cplusplus
}
#endif
]
,
[
  int i;
  int fds;
  i = select(1, &fds, &fds, &fds, NULL);
],
eval "ac_cv_prototype_intp_select=yes", eval "ac_cv_prototype_intp_select=no")])])
if eval "test \"`echo $ac_cv_prototype_intp_select`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_prototype=HAVE_INTP_SELECT
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_prototype)
  ifelse([$2], , :, [$2])
else
  AC_MSG_RESULT(no)
  ifelse([$3], , , [$3])
fi
])



dnl AC_TRY_COMPILE_AND_LINK compiles a Source file into an object file
dnl  and links the object file. This can create a different behaviour
dnl  than compiling and linking the object file directly (e.g. 
dnl  Sun C++ 3.0.1 with template functions)

dnl AC_TRY_COMPILE_AND_LINK(SOURCE, MAIN_BODY 
dnl                         [, ACTION-IF-FOUND [,ACTION-IF-NOT-FOUND]])
AC_DEFUN(AC_TRY_COMPILE_AND_LINK,
[ac_link_o='${CXX-g++} -o conftest $CXXFLAGS $CPPFLAGS $LDFLAGS conftest.o $LIBS 1>&AC_FD_CC'
cat > conftest.$ac_ext <<EOF
dnl This sometimes fails to find confdefs.h, for some reason.
dnl [#]line __oline__ "[$]0"
[#]line __oline__ "configure"
#include "confdefs.h"
[$1]
int main() {
[$2]
; return 0; }
EOF
if AC_TRY_EVAL(ac_compile); then
  if AC_TRY_EVAL(ac_link_o); then
    ifelse([$3], , :, [rm -rf conftest*
    $3])
  else
    echo "configure: failed link was:" >&AC_FD_CC
    cat conftest.$ac_ext >&AC_FD_CC
    ifelse([$4], , , [  rm -rf conftest*
      $4
    ])dnl
  fi
else
  echo "configure: failed compile was:" >&AC_FD_CC
  cat conftest.$ac_ext >&AC_FD_CC
ifelse([$4], , , [  rm -rf conftest*
  $4
])dnl
fi
rm -f conftest*])

dnl AC_CHECK_STD_NAMESPACE checks if the C++-Compiler supports the
dnl   standard name space.

dnl AC_CHECK_STD_NAMESPACE
AC_DEFUN(AC_CHECK_STD_NAMESPACE,
[AC_MSG_CHECKING([for C++ standard namespace])
AH_TEMPLATE([HAVE_STD_NAMESPACE], [Define if ANSI standard C++ includes use std namespace])
AC_CACHE_VAL(ac_cv_check_std_namespace,
[AC_TRY_COMPILE_AND_LINK([
#include <iostream>
using namespace std;
],[
  cout << "Hello World" << endl;
], eval "ac_cv_check_std_namespace=yes", eval "ac_cv_check_std_namespace=no")dnl
])dnl
if eval "test \"`echo '$ac_cv_check_std_namespace'`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_std_namespace=HAVE_STD_NAMESPACE
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_std_namespace)
else
  AC_MSG_RESULT(no)
fi
])

 

dnl AC_CHECK_CLASS_TEMPLATE checks if the C++-Compiler is capable of
dnl   using class templates in the easiest form i. e. all methods are
dnl   inline, no template methods and no typedefs in the class

dnl AC_CHECK_CLASS_TEMPLATE
AC_DEFUN(AC_CHECK_CLASS_TEMPLATE,
[AC_MSG_CHECKING([for C++ class template])
AH_TEMPLATE([HAVE_CLASS_TEMPLATE], [Define if your C++ compiler can work with class templates])
AC_CACHE_VAL(ac_cv_check_class_template,
[AC_TRY_COMPILE_AND_LINK([
template <class T>
class x
{
private:
	T a;
public:
	void set(T i) { a = i; }
	x(T i) { set(i); }
	T get() { return a; }
};
],[
  int i;
  x<int> a(4);
  i = a.get();
  a.set(18);
  i = a.get();
  a.set(i-1);
], eval "ac_cv_check_class_template=yes", eval "ac_cv_check_class_template=no")dnl
])dnl
if eval "test \"`echo '$ac_cv_check_class_template'`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_class_template=HAVE_CLASS_TEMPLATE
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_class_template)
else
  AC_MSG_RESULT(no)
fi
])

 

dnl AC_CHECK_FUNCTION_TEMPLATE checks if the C++-Compiler is capable of
dnl   using function templates.

dnl AC_CHECK_FUNCTION_TEMPLATE
AC_DEFUN(AC_CHECK_FUNCTION_TEMPLATE,
[AC_MSG_CHECKING([for C++ function template])
AH_TEMPLATE([HAVE_FUNCTION_TEMPLATE], [Define if your C++ compiler can work with function templates])
AC_CACHE_VAL(ac_cv_check_function_template,
[AC_TRY_COMPILE_AND_LINK([
template <class T>
int f(T* a)
{
 if (a) return 1;
 return 0;
}
],[
  int i, a;
  i = f(&a);
], eval "ac_cv_check_function_template=yes", eval "ac_cv_check_function_template=no")dnl
])dnl
if eval "test \"`echo '$ac_cv_check_function_template'`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_function_template=HAVE_FUNCTION_TEMPLATE
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_function_template)
else
  AC_MSG_RESULT(no)
fi
])


dnl AC_CHECK_STATIC_TEMPLATE_METHOD checks if the C++-Compiler is capable of
dnl   using static methods in template classes 

dnl AC_CHECK_STATIC_TEMPLATE_METHOD
AC_DEFUN(AC_CHECK_STATIC_TEMPLATE_METHOD,
[AC_MSG_CHECKING([for C++ static methods in class templates])
AH_TEMPLATE([HAVE_STATIC_TEMPLATE_METHOD], [Define if your C++ compiler can work with static methods in class templates])
AC_CACHE_VAL(ac_cv_check_static_template_method,
[AC_TRY_COMPILE_AND_LINK([
void additive(int & i)
{
        i++;
}

template <class T, class Function>
class x
{
public:
	x() { }
	static void do1(Function f, int & a) { f(a); }
        static void do2(T b) { T a = b; };
};
],[
  int a = 1;
  x<int, void (*)(int &)>::do1(additive, a);
  x<int, void (*)(int &)>::do2(a);
], eval "ac_cv_check_static_template_method=yes", eval "ac_cv_check_static_template_method=no")dnl
])dnl
if eval "test \"`echo '$ac_cv_check_static_template_method'`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_static_template_method=HAVE_STATIC_TEMPLATE_METHOD
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_static_template_method)
else
  AC_MSG_RESULT(no)
fi
])


dnl AC_CHECK_EXPLICIT_TEMPLATE_SPECIALIZATION checks if the C++-Compiler
dnl   supports the explicit template specialization syntax, i.e.
dnl     template<> int classname<int>::functionname()

dnl AC_CHECK_EXPLICIT_TEMPLATE_SPECIALIZATION
AC_DEFUN(AC_CHECK_EXPLICIT_TEMPLATE_SPECIALIZATION,
[AC_MSG_CHECKING([for C++ explicit template specialization syntax])
AH_TEMPLATE([HAVE_EXPLICIT_TEMPLATE_SPECIALIZATION], [Define if your C++ compiler supports the explicit template specialization syntax])
AC_CACHE_VAL(ac_cv_check_explicit_template_specialization,
[AC_TRY_COMPILE([
template<class T>
class X
{
  public:
    int fn();
};

template<>
int X<int>::fn()
{
    return 0;
}
],[X<int> x], eval "ac_cv_check_explicit_template_specialization=yes", eval "ac_cv_check_explicit_template_specialization=no")dnl
])dnl
if eval "test \"`echo '$ac_cv_check_explicit_template_specialization'`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_explicit_template_specialization=HAVE_EXPLICIT_TEMPLATE_SPECIALIZATION
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_explicit_template_specialization)
else
  AC_MSG_RESULT(no)
fi
])


dnl AC_CHECK_GNU_LIBTOOL checks whether libtool is GNU libtool.
dnl   This macro requires that 'libtool' exists in the current path,
dnl   i.e. AC_CHECK_PROGS(LIBTOOL, libtool, :) should be executed and evaluated
dnl   before performing this test.
dnl   If libtool is identified as GNU libtool, the environment variable $ac_cv_check_gnu_libtool
dnl   is set to the value "yes", otherwise to "no".
dnl
dnl AC_CHECK_GNU_LIBTOOL
AC_DEFUN(AC_CHECK_GNU_LIBTOOL,
[AC_MSG_CHECKING([whether libtool is GNU libtool])
AC_CACHE_VAL(ac_cv_check_gnu_libtool,
[ ac_cv_check_gnu_libtool=`libtool --version 2>/dev/null | grep "GNU libtool" | sed -e "s/.*\(GNU libtool\).*/\1/" `
  if test "${ac_cv_check_gnu_libtool}set" = "GNU libtoolset" ; then
    ac_cv_check_gnu_libtool=yes
  else
    ac_cv_check_gnu_libtool=no
  fi
])
  if test $ac_cv_check_gnu_libtool = yes ; then
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
  fi
])



dnl AC_CHECK_HEADER_WITH_INCLUDES works like AC_CHECK_HEADER but allows to specify
dnl additional code to be put into the test program before the #include statement
dnl generated by AC_CHECK_HEADER.
dnl Typically this additional code will be additional #include statements.
dnl For example, on Irix 6, <netinet/tcp.h> cannot be included if <netinet/in.h> is not
dnl included before (leads to parse error).
dnl
dnl AC_CHECK_HEADER_WITH_INCLUDES(HEADER-FILE, [INCLUDES, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
AC_DEFUN(AC_CHECK_HEADER_WITH_INCLUDES,
[dnl Do the transliteration at runtime so arg 1 can be a shell variable.
ac_safe=`echo "$1" | sed 'y%./+-%__p_%'`
AC_MSG_CHECKING([for $1])
AC_CACHE_VAL(ac_cv_header_$ac_safe,
[AC_TRY_CPP([$2
#include <$1>], eval "ac_cv_header_$ac_safe=yes",
  eval "ac_cv_header_$ac_safe=no")])dnl
if eval "test \"`echo '$ac_cv_header_'$ac_safe`\" = yes"; then
  AC_MSG_RESULT(yes)
  ifelse([$3], , :, [$3])
else
  AC_MSG_RESULT(no)
ifelse([$4], , , [$4
])dnl
fi
])

dnl AC_CHECK_HEADERS_WITH_INCLUDES works like AC_CHECK_HEADERS but allows to specify
dnl additional code to be put into the test program before the #include statement
dnl generated by AC_CHECK_HEADERS.
dnl Typically this additional code will be additional #include statements.
dnl For example, on Irix 6, <netinet/tcp.h> cannot be included if <netinet/in.h> is not
dnl included before (leads to parse error).
dnl
dnl AC_CHECK_HEADERS_WITH_INCLUDES(HEADER-FILE..., [INCLUDES, [, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
AC_DEFUN(AC_CHECK_HEADERS_WITH_INCLUDES,
[for ac_hdr in $1
do
AC_CHECK_HEADER_WITH_INCLUDES($ac_hdr, $2,
[changequote(, )dnl
  ac_tr_hdr=HAVE_`echo $ac_hdr | sed 'y%abcdefghijklmnopqrstuvwxyz./-%ABCDEFGHIJKLMNOPQRSTUVWXYZ___%'`
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_hdr) $3], $4)dnl
done
])


dnl AC_CHECK_TCP_H
dnl checks for the presence of three system include files:
dnl    <netinet/in_systm.h>
dnl    <netinet/in.h>
dnl    <netinet/tcp.h>
dnl On certain systems like Irix or NeXT these are interdependent, i.e.
dnl tcp.h can only be included if in.h is included before. This script
dnl accounts for these dependencies.
dnl
dnl Note: GNU autoheader recognizes the includes tested by this macro.
dnl   No manual intervention required.
dnl
AC_DEFUN(AC_CHECK_TCP_H,
[AC_CHECK_HEADERS(netinet/in_systm.h)

if eval "test \"`echo '$ac_cv_header_netinet_in_systm_h'`\" = yes"; then
  AC_CHECK_HEADERS_WITH_INCLUDES(netinet/in.h,
#include <netinet/in_systm.h>
)
  if eval "test \"`echo '$ac_cv_header_netinet_in_h'`\" = yes"; then
    AC_CHECK_HEADERS_WITH_INCLUDES(netinet/tcp.h,
#include <netinet/in_systm.h>
#include <netinet/in.h>
)
  else
    AC_CHECK_HEADERS_WITH_INCLUDES(netinet/tcp.h,
#include <netinet/in_systm.h>
)
  fi
else
  AC_CHECK_HEADERS(netinet/in.h)
  if eval "test \"`echo '$ac_cv_header_netinet_in_h'`\" = yes"; then
    AC_CHECK_HEADERS_WITH_INCLUDES(netinet/tcp.h,
#include <netinet/in.h>
)
  else
    AC_CHECK_HEADERS(netinet/tcp.h)
  fi
fi
])

dnl AC_CHECK_INTP_ACCEPT checks if the prototype for accept()
dnl   specifies arguments 2-4 to be int* instead of size_t *.

dnl AC_CHECK_INTP_ACCEPT(HEADER-FILE..., ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
AC_DEFUN(AC_CHECK_INTP_ACCEPT,
[AC_MSG_CHECKING([ifelse([$1], , [if accept() needs int* parameters], 
[if accept() needs int* parameters (in $1)])])
AH_TEMPLATE([HAVE_INTP_ACCEPT], [Define if your system declares argument 3 of accept()
   as int * instead of size_t * or socklen_t *])
ifelse([$1], , [ac_includes=""
],
[ac_includes=""
for ac_header in $1
do
  ac_includes="$ac_includes
#include<$ac_header>"
done])
AC_CACHE_VAL(ac_cv_prototype_intp_accept,
[AC_TRY_COMPILE(
[#ifdef __cplusplus
extern "C" {
#endif
$ac_includes
#ifdef __cplusplus
}
#endif
]
,
[
  int i;
  struct sockaddr *addr;
  size_t addrlen;

  addr = 0;
  addrlen = 0;
  i = accept(1, addr, &addrlen);
],
eval "ac_cv_prototype_intp_accept=no", 
[AC_TRY_COMPILE(
[#ifdef __cplusplus
extern "C" {
#endif
$ac_includes
#ifdef __cplusplus
}
#endif
]
,
[
  int i;
  struct sockaddr *addr;
  int addrlen;

  addr = 0;
  addrlen = 0;
  i = accept(1, addr, &addrlen);
],
eval "ac_cv_prototype_intp_accept=yes", eval "ac_cv_prototype_intp_accept=no")])])
if eval "test \"`echo $ac_cv_prototype_intp_accept`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_prototype=HAVE_INTP_ACCEPT
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_prototype)
  ifelse([$2], , :, [$2])
else
  AC_MSG_RESULT(no)
  ifelse([$3], , , [$3])
fi
])




dnl AC_CHECK_PTHREAD_OPTION checks whether the compiler requires the 
dnl -pthread option to correctly link code containing posix thread calls.
dnl This is true for example on FreeBSD.
dnl This test assumes that <pthread.h> is available.

dnl If the test is positive, -pthread is added to CFLAGS and CXXFLAGS.

dnl AC_CHECK_PTHREAD_OPTION
AC_DEFUN(AC_CHECK_PTHREAD_OPTION,
[AC_MSG_CHECKING(whether compiler requires -pthread option for posix threads)
REQUIRES_PTHREAD_OPTION="no"
AC_TRY_LINK(
[#ifdef __cplusplus
extern "C" {
#endif
]
#include <pthread.h>
[#ifdef __cplusplus
}
#endif
]
,
[
  (void) pthread_create(NULL, NULL, NULL, NULL);
], , 
SAVE_CXXFLAGS="$CXXFLAGS"
SAVE_CFLAGS="$CFLAGS"
CXXFLAGS="-pthread $CXXFLAGS"
CFLAGS="-pthread $CFLAGS"
AC_TRY_LINK(
[#ifdef __cplusplus
extern "C" {
#endif
]
#include <pthread.h>
[#ifdef __cplusplus
}
#endif
]
,
[
  (void) pthread_create(NULL, NULL, NULL, NULL);
], 
REQUIRES_PTHREAD_OPTION="yes"
, ) 
CXXFLAGS="$SAVE_CXXFLAGS"
CFLAGS="$SAVE_CFLAGS"
)

if test $REQUIRES_PTHREAD_OPTION = yes ; then
  AC_MSG_RESULT([yes])
  CXXFLAGS="-pthread $CXXFLAGS"
  CFLAGS="-pthread $CFLAGS"
else
  AC_MSG_RESULT([no])
fi
])



dnl AC_MY_C_INLINE works like the standard script AC_C_INLINE
dnl but defines C_INLINE instead of redefining "inline" directly.

AC_DEFUN(AC_MY_C_INLINE,
[AH_TEMPLATE([C_INLINE], [Define to the inline keyword supported by the C compiler, if any, or to the empty string])
AC_CACHE_CHECK([for inline], ac_cv_my_c_inline,
[ac_cv_my_c_inline=no
for ac_kw in inline __inline__ __inline; do
  AC_TRY_COMPILE(, [} $ac_kw foo() {], [ac_cv_my_c_inline=$ac_kw; break])
done
])
case "$ac_cv_my_c_inline" in
  inline | yes) AC_DEFINE(C_INLINE, inline) ;;
  no) AC_DEFINE(C_INLINE, ) ;;
  *)  AC_DEFINE_UNQUOTED(C_INLINE, $ac_cv_my_c_inline) ;;
esac
])


dnl AC_MY_C_CONST works like the standard script AC_C_CONST
dnl but defines HAVE_C_CONST instead of redefining "const" directly.

AC_DEFUN(AC_MY_C_CONST,
[AH_TEMPLATE([HAVE_C_CONST], [Define if "const" is supported by the C compiler])
dnl This message is consistent in form with the other checking messages,
dnl and with the result message.
AC_CACHE_CHECK([for working const], ac_cv_my_c_const,
[AC_TRY_COMPILE(,
changequote(<<, >>)dnl
<<
/* Ultrix mips cc rejects this.  */
typedef int charset[2]; const charset x;
/* SunOS 4.1.1 cc rejects this.  */
char const *const *ccp;
char **p;
/* NEC SVR4.0.2 mips cc rejects this.  */
struct point {int x, y;};
static struct point const zero = {0,0};
/* AIX XL C 1.02.0.0 rejects this.
   It does not let you subtract one const X* pointer from another in an arm
   of an if-expression whose if-part is not a constant expression */
const char *g = "string";
ccp = &g + (g ? g-g : 0);
/* HPUX 7.0 cc rejects these. */
++ccp;
p = (char**) ccp;
ccp = (char const *const *) p;
{ /* SCO 3.2v4 cc rejects this.  */
  char *t;
  char const *s = 0 ? (char *) 0 : (char const *) 0;

  *t++ = 0;
}
{ /* Someone thinks the Sun supposedly-ANSI compiler will reject this.  */
  int x[] = {25, 17};
  const int *foo = &x[0];
  ++foo;
}
{ /* Sun SC1.0 ANSI compiler rejects this -- but not the above. */
  typedef const int *iptr;
  iptr p = 0;
  ++p;
}
{ /* AIX XL C 1.02.0.0 rejects this saying
     "k.c", line 2.27: 1506-025 (S) Operand must be a modifiable lvalue. */
  struct s { int j; const int *ap[3]; };
  struct s *b; b->j = 5;
}
{ /* ULTRIX-32 V3.1 (Rev 9) vcc rejects this */
  const int foo = 10;
}
>>,
changequote([, ])dnl
ac_cv_my_c_const=yes, ac_cv_my_c_const=no)])
if test $ac_cv_my_c_const = yes; then
  AC_DEFINE_UNQUOTED(HAVE_C_CONST)
fi
])


dnl AC_MY_C_CHAR_UNSIGNED works like the standard script AC_C_CHAR_UNSIGNED
dnl but defines C_CHAR_UNSIGNED instead of __CHAR_UNSIGNED__.

AC_DEFUN(AC_MY_C_CHAR_UNSIGNED,
[AH_TEMPLATE([C_CHAR_UNSIGNED], [Define if char is unsigned on the C compiler])
AC_CACHE_CHECK(whether char is unsigned, ac_cv_my_c_char_unsigned,
[if test "$GCC" = yes; then
  # GCC predefines this symbol on systems where it applies.
AC_EGREP_CPP(yes,
[#ifdef __CHAR_UNSIGNED__
  yes
#endif
], ac_cv_my_c_char_unsigned=yes, ac_cv_my_c_char_unsigned=no)
else
AC_TRY_RUN(
[/* volatile prevents gcc2 from optimizing the test away on sparcs.  */
#if !defined(__STDC__) || __STDC__ != 1
#define volatile
#endif
main() {
  volatile char c = 255; exit(c < 0);
}], ac_cv_my_c_char_unsigned=yes, ac_cv_my_c_char_unsigned=no)
fi])
if test $ac_cv_my_c_char_unsigned = yes ; then
  AC_DEFINE(C_CHAR_UNSIGNED)
fi
])


dnl AC_MY_C_RIGHTSHIFT_UNSIGNED checks whether the right shift operation
dnl is unsigned and, if yes, defines C_RIGHTSHIFT_UNSIGNED.

AC_DEFUN(AC_MY_C_RIGHTSHIFT_UNSIGNED,
[AH_TEMPLATE([C_RIGHTSHIFT_UNSIGNED], [Define if >> is unsigned on the C compiler])
AC_CACHE_CHECK(whether right shift is unsigned, ac_cv_my_c_rightshift_unsigned,
[
AC_TRY_RUN(
[/* See whether right-shift on a long is signed or not. */
int is_shifting_signed (long arg)
{
  long res = arg >> 4;

  if (res == -0x7F7E80CL) {	/* expected result for signed shift */
    return 1;			/* right shift is signed */
  }
  /* see if unsigned-shift hack will fix it. */
  /* we can't just test exact value since it depends on width of long... */
  res |= (~0L) << (32-4);
  if (res == -0x7F7E80CL) {	/* expected result now? */
    return 0;			/* right shift is unsigned */
  }
  /* Right shift isn't acting as I expect it to, try it with unsigned anyway */
  return 0;
}

int main()
{
  exit(is_shifting_signed(-0x7F7E80B1L));
}
], ac_cv_my_c_rightshift_unsigned=yes, ac_cv_my_c_rightshift_unsigned=no)
])
if test $ac_cv_my_c_rightshift_unsigned = yes ; then
  AC_DEFINE(C_CHAR_UNSIGNED)
fi
])


dnl AC_CHECK_IOS_NOCREATE checks if the flag ios::nocreate is defined.

dnl AC_CHECK_IOS_NOCREATE(IOS-Name, header [, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
AC_DEFUN(AC_CHECK_IOS_NOCREATE,
[
AC_MSG_CHECKING([declaration of ios::nocreate (in $2)])
AH_TEMPLATE([HAVE_IOS_NOCREATE], [Define if your system defines ios::nocreate in iostream.h])
ac_cv_declaration=ac_cv_declaration_ios_nocreate
AC_CACHE_VAL($ac_cv_declaration,
[AC_TRY_COMPILE([
#include <$2>
], [ifstream file("name", $1::nocreate)] ,eval "$ac_cv_declaration=yes", eval "$ac_cv_declaration=no")])dnl
if eval "test \"\$$ac_cv_declaration\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_declaration=HAVE_IOS_NOCREATE
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_declaration)
  ifelse([$3], , :, [$3])
else
  AC_MSG_RESULT(no)
ifelse([$4], , , [$4
])dnl
fi
unset ac_cv_declaration
])



dnl AC_CHECK_OLD_READDIR_R checks if there is a function readdir_r
dnl   conforming to the Posix 1.c Draft 6 interface, i.e.
dnl     struct dirent *readdir_r(DIR *dirp, struct dirent *entry);
dnl   instead of the Posix 1.c interface, i.e.
dnl     int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);
dnl
dnl  The test macro AC_HEADER_DIRENT must be run before this test!
dnl
dnl AC_CHECK_OLD_READDIR_R([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
AC_DEFUN(AC_CHECK_OLD_READDIR_R,
[
AC_MSG_CHECKING([if declaration of readdir_r conforms to Posix 1.c draft 6])
AH_TEMPLATE([HAVE_OLD_READDIR_R], [Define if your system supports readdir_r with the obsolete
   Posix 1.c draft 6 declaration (2 arguments) instead of the
   Posix 1.c declaration with 3 arguments.])
ac_cv_result=ac_cv_old_readdir_r
AC_CACHE_VAL($ac_cv_result,
[AC_TRY_COMPILE([
#ifdef __cplusplus
extern "C" {
#endif
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#else
#define dirent direct
#ifdef HAVE_SYS_NDIR_H
#include <sys/ndir.h>
#endif
#ifdef HAVE_SYS_DIR_H
#include <sys/dir.h>
#endif
#ifdef HAVE_NDIR_H
#include <ndir.h>
#endif
#endif
#ifdef __cplusplus
}
#endif
], [
  DIR *dirp;
  struct dirent *dp;
  dp = readdir_r(dirp, dp);
], eval "$ac_cv_result=yes", eval "$ac_cv_result=no")])dnl
if eval "test \"\$$ac_cv_result\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_cv_result=HAVE_OLD_READDIR_R
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_cv_result)
  ifelse([$1], , , [$1
])dnl
else
  AC_MSG_RESULT(no)
  ifelse([$2], , , [$2
])dnl
fi
unset ac_cv_result
])


dnl AC_CHECK_INTP_GETSOCKOPT checks if the prototype for getsockopt()
dnl   specifies arguments 5 to be int* instead of size_t *.

dnl AC_CHECK_INTP_GETSOCKOPT(HEADER-FILE..., ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
AC_DEFUN(AC_CHECK_INTP_GETSOCKOPT,
[AC_MSG_CHECKING([ifelse([$1], , [if getsockopt() needs int* parameters], 
[if getsockopt() needs int* parameters (in $1)])])
AH_TEMPLATE([HAVE_INTP_GETSOCKOPT], [Define if your system declares argument 5 of getsockopt()
   as int * instead of size_t * or socklen_t])
ifelse([$1], , [ac_includes=""
],
[ac_includes=""
for ac_header in $1
do
  ac_includes="$ac_includes
#include<$ac_header>"
done])
AC_CACHE_VAL(ac_cv_prototype_intp_getsockopt,
[AC_TRY_COMPILE(
[#ifdef __cplusplus
extern "C" {
#endif
$ac_includes
#ifdef __cplusplus
}
#endif
]
,
[
  int i;
  size_t optlen;
  i = getsockopt(0, 0, 0, 0, &optlen);
],
eval "ac_cv_prototype_intp_getsockopt=no", 
[AC_TRY_COMPILE(
[#ifdef __cplusplus
extern "C" {
#endif
$ac_includes
#ifdef __cplusplus
}
#endif
]
,
[
  int i;
  int optlen;
  i = getsockopt(0, 0, 0, 0, &optlen);
],
eval "ac_cv_prototype_intp_getsockopt=yes", eval "ac_cv_prototype_intp_getsockopt=no")])])
if eval "test \"`echo $ac_cv_prototype_intp_getsockopt`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_prototype=HAVE_INTP_GETSOCKOPT
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_prototype)
  ifelse([$2], , :, [$2])
else
  AC_MSG_RESULT(no)
  ifelse([$3], , , [$3])
fi
])


dnl AC_CHECK_ELLIPSE_SIGNAL_HANDLER checks if the prototype for the
dnl   callback function passed to signal() needs an ellipse (...)
dnl   as parameter.  Needed for example on Irix 5.
dnl   The header files for signal() have to be specified.

dnl AC_CHECK_ELLIPSE_SIGNAL_HANDLER(HEADER-FILE..., ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
AC_DEFUN(AC_CHECK_ELLIPSE_SIGNAL_HANDLER,
[AC_MSG_CHECKING([ifelse([$1], , [if signal() callback needs ellipse], 
[if signal() callback needs ellipse (in $1)])])
AH_TEMPLATE([SIGNAL_HANDLER_WITH_ELLIPSE], [Define if signal handlers need ellipse (...) parameters])
ifelse([$1], , [ac_includes=""
],
[ac_includes=""
for ac_header in $1
do
  ac_includes="$ac_includes
#include<$ac_header>"
done])
AC_CACHE_VAL(ac_cv_signal_handler_with_ellipse,
[AC_TRY_COMPILE(
[#ifdef __cplusplus
extern "C" {
#endif
$ac_includes

  void signal_handler(int)
  {
  }
#ifdef __cplusplus
}
#endif
]
,
[
 (void) signal(0, signal_handler);
],
eval "ac_cv_signal_handler_with_ellipse=no", 
[AC_TRY_COMPILE(
[#ifdef __cplusplus
extern "C" {
#endif
$ac_includes

  void signal_handler(...)
  {
  }
#ifdef __cplusplus
}
#endif
]
,
[
 (void) signal(0, signal_handler);
],
eval "ac_cv_signal_handler_with_ellipse=yes", eval "ac_cv_signal_handler_with_ellipse=no")])])
if eval "test \"`echo $ac_cv_signal_handler_with_ellipse`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_prototype=SIGNAL_HANDLER_WITH_ELLIPSE
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_prototype)
  ifelse([$2], , :, [$2])
else
  AC_MSG_RESULT(no)
  ifelse([$3], , , [$3])
fi
])



dnl AC_INCLUDE_MATH_H_AS_CXX checks if <math.h> must be included as a C++ 
dnl   include file (i.e. without extern "C"). Some sytems (Win32, HP/UX 10)
dnl   use C++ language features in <math.h>

dnl AC_INCLUDE_MATH_H_AS_CXX(HEADER-FILE..., ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
AC_DEFUN(AC_INCLUDE_MATH_H_AS_CXX,
[AC_MSG_CHECKING([if <math.h> fails if included extern "C"])
AH_TEMPLATE([INCLUDE_MATH_H_AS_CXX], [Define if <math.h> fails if included extern "C"])
AC_CACHE_VAL(ac_cv_include_math_h_as_cxx,
[AC_TRY_COMPILE([
extern "C"
{
#include<math.h>
}
],
[
],
eval "ac_cv_include_math_h_as_cxx=no", 
[AC_TRY_COMPILE([
#include<math.h>
],
[
],
eval "ac_cv_include_math_h_as_cxx=yes", eval "ac_cv_include_math_h_as_cxx=no")])])
if eval "test \"`echo $ac_cv_include_math_h_as_cxx`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_prototype=INCLUDE_MATH_H_AS_CXX
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_prototype)
  ifelse([$2], , :, [$2])
else
  AC_MSG_RESULT(no)
  ifelse([$3], , , [$3])
fi
])


dnl AC_CHECK_CXX_BOOL checks if bool is a built-in C++ type
dnl   (which is not the case on older compilers).

dnl AC_CHECK_CXX_BOOL(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
AC_DEFUN(AC_CHECK_CXX_BOOL,
[AC_MSG_CHECKING([if bool is built-in type])
AH_TEMPLATE([HAVE_CXX_BOOL], [Define if bool is a built-in type])
AC_CACHE_VAL(ac_cv_have_cxx_bool,
[AC_TRY_COMPILE([],[
bool b1 = true;
bool b2 = false;
],
eval "ac_cv_have_cxx_bool=yes", 
eval "ac_cv_have_cxx_bool=no")])
if eval "test \"`echo $ac_cv_have_cxx_bool`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_prototype=HAVE_CXX_BOOL
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_prototype)
  ifelse([$1], , :, [$1])
else
  AC_MSG_RESULT(no)
  ifelse([$2], , , [$2])
fi
])


dnl AC_CHECK_CXX_VOLATILE checks if volatile is a built-in C++ keyword
dnl   (which is not the case on older compilers).

dnl AC_CHECK_CXX_VOLATILE(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
AC_DEFUN(AC_CHECK_CXX_VOLATILE,
[AC_MSG_CHECKING([if volatile is known keyword])
AH_TEMPLATE([HAVE_CXX_VOLATILE], [Define if volatile is a known keyword])
AC_CACHE_VAL(ac_cv_have_cxx_volatile,
[AC_TRY_COMPILE([],[
volatile int i=0;
],
eval "ac_cv_have_cxx_volatile=yes", 
eval "ac_cv_have_cxx_volatile=no")])
if eval "test \"`echo $ac_cv_have_cxx_volatile`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_prototype=HAVE_CXX_VOLATILE
changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_prototype, ,[Define if volatile is a known keyword])
  ifelse([$1], , :, [$1])
else
  AC_MSG_RESULT(no)
  ifelse([$2], , , [$2])
fi
])


dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/ac_cxx_typename.html
dnl
AC_DEFUN([AC_CXX_TYPENAME],
[AH_TEMPLATE([HAVE_TYPENAME], [Define if typename is a known keyword])
AC_CACHE_CHECK(whether the compiler recognizes typename,
ac_cv_cxx_typename,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([template<typename T>class X {public:X(){}};],
[X<float> z; return 0;],
 ac_cv_cxx_typename=yes, ac_cv_cxx_typename=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_typename" = yes; then
  AC_DEFINE(HAVE_TYPENAME,,[define if the compiler recognizes typename])
fi
])


dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/ac_cxx_const_cast.html
dnl
AC_DEFUN([AC_CXX_CONST_CAST],
[AH_TEMPLATE([HAVE_CONST_CAST], [Define if the compiler supports const_cast<>])
AC_CACHE_CHECK(whether the compiler supports const_cast<>,
ac_cv_cxx_const_cast,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE(,[int x = 0;const int& y = x;int& z = const_cast<int&>(y);return z;],
 ac_cv_cxx_const_cast=yes, ac_cv_cxx_const_cast=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_const_cast" = yes; then
  AC_DEFINE(HAVE_CONST_CAST,,[define if the compiler supports const_cast<>])
fi
])


dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/ac_cxx_dynamic_cast.html
dnl
AC_DEFUN([AC_CXX_DYNAMIC_CAST],
[AH_TEMPLATE([HAVE_DYNAMIC_CAST], [Define if the compiler supports dynamic_cast<>])
AC_CACHE_CHECK(whether the compiler supports dynamic_cast<>,
ac_cv_cxx_dynamic_cast,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <typeinfo>
class Base { public : Base () {} virtual void f () = 0;};
class Derived : public Base { public : Derived () {} virtual void f () {} };],[
Derived d; Base& b=d; return dynamic_cast<Derived*>(&b) ? 0 : 1;],
 ac_cv_cxx_dynamic_cast=yes, ac_cv_cxx_dynamic_cast=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_dynamic_cast" = yes; then
  AC_DEFINE(HAVE_DYNAMIC_CAST,,[define if the compiler supports dynamic_cast<>])
fi
])


dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/ac_cxx_reinterpret_cast.html
dnl
AC_DEFUN([AC_CXX_REINTERPRET_CAST],
[AH_TEMPLATE([HAVE_REINTERPRET_CAST], [Define if the compiler supports reinterpret_cast<>])
AC_CACHE_CHECK(whether the compiler supports reinterpret_cast<>,
ac_cv_cxx_reinterpret_cast,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <typeinfo>
class Base { public : Base () {} virtual void f () = 0;};
class Derived : public Base { public : Derived () {} virtual void f () {} };
class Unrelated { public : Unrelated () {} };
int g (Unrelated&) { return 0; }],[
Derived d;Base& b=d;Unrelated& e=reinterpret_cast<Unrelated&>(b);return g(e);],
 ac_cv_cxx_reinterpret_cast=yes, ac_cv_cxx_reinterpret_cast=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_reinterpret_cast" = yes; then
  AC_DEFINE(HAVE_REINTERPRET_CAST,,
            [define if the compiler supports reinterpret_cast<>])
fi
])


dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/ac_cxx_static_cast.html
dnl
AC_DEFUN([AC_CXX_STATIC_CAST],
[AH_TEMPLATE([HAVE_STATIC_CAST], [Define if the compiler supports static_cast<>])
AC_CACHE_CHECK(whether the compiler supports static_cast<>,
ac_cv_cxx_static_cast,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <typeinfo>
class Base { public : Base () {} virtual void f () = 0; };
class Derived : public Base { public : Derived () {} virtual void f () {} };
int g (Derived&) { return 0; }],[
Derived d; Base& b = d; Derived& s = static_cast<Derived&> (b); return g (s);],
 ac_cv_cxx_static_cast=yes, ac_cv_cxx_static_cast=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_static_cast" = yes; then
  AC_DEFINE(HAVE_STATIC_CAST,,
            [define if the compiler supports static_cast<>])
fi
])


dnl AC_CXX_STD_NOTHROW checks if the compiler supports non-throwing new using
dnl std::nothrow.

AC_DEFUN([AC_CXX_STD_NOTHROW],
[AH_TEMPLATE([HAVE_STD__NOTHROW], [Define if the compiler supports std::nothrow])
AC_CACHE_CHECK(whether the compiler supports std::nothrow,
ac_cv_cxx_std_nothrow,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <new>],[int *i = new (std::nothrow) int],
 ac_cv_cxx_std_nothrow=yes, ac_cv_cxx_std_nothrow=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_std_nothrow" = yes; then
  AC_DEFINE(HAVE_STD__NOTHROW,, [Define if the compiler supports std::nothrow])
fi
])


dnl AC_LIBTIFF_LZW_COMPRESSION checks if libtiff supports LZW compression.

AC_DEFUN([AC_LIBTIFF_LZW_COMPRESSION],
[AH_TEMPLATE([HAVE_LIBTIFF_LZW_COMPRESSION], [Define if libtiff supports LZW compression])
AC_CACHE_CHECK(whether libtiff supports LZW compression,
ac_cv_libtiff_lzw_compression,
[AC_TRY_RUN(
changequote({{, }})dnl
{{
extern "C" {
#include <tiffio.h>
}

int main()
{
  const char *data[256];
  for (int j=0; j<256; ++j) data[j]= 0;        

  int OK = 1;
  TIFF *tif = TIFFOpen("lzwtest.tif", "w");
  if (tif)
  {
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 16);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 16);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
    TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, "unnamed");
    TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, "test");
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 512);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    for (unsigned long i=0; (i < 16) && OK; i++)
    {
      if (TIFFWriteScanline(tif, data + (i << 4), i, 0) < 0) OK = 0;
    }
    TIFFFlushData(tif);
    TIFFClose(tif);
  }
  if (OK) return 0; else return 10;
}

}}
changequote([, ])dnl
 , ac_cv_libtiff_lzw_compression=yes, ac_cv_libtiff_lzw_compression=no, ac_cv_libtiff_lzw_compression=no)
])
if test "$ac_cv_libtiff_lzw_compression" = yes; then
  AC_DEFINE(HAVE_LIBTIFF_LZW_COMPRESSION,, [Define if libtiff supports LZW compression])
fi
])


dnl AC_CXX_LIBC_H_EXTERN_C checks if <libc.h> and <math.h> cause a problem if
dnl   libc.h is included extern "C" and math.h is not. This is the case on QNX 6.2.x

AC_DEFUN([AC_CXX_LIBC_H_EXTERN_C],
[AH_TEMPLATE([INCLUDE_LIBC_H_AS_CXX], [Define if libc.h should be treated as a C++ header])
AC_CACHE_CHECK(whether libc.h should be treated as a C++ header,
ac_cv_cxx_libc_h_is_cxx,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <math.h>
extern "C" {
#include <libc.h>
}],[int i = 0],
 ac_cv_cxx_libc_h_is_cxx=no, ac_cv_cxx_libc_h_is_cxx=yes)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_libc_h_is_cxx" = yes; then
  AC_DEFINE(INCLUDE_LIBC_H_AS_CXX,, [Define if libc.h should be treated as a C++ header])
fi
])


dnl AC_CHECK_POINTER_TYPE checks if a given type is a pointer type.
dnl   If header file(s) are given as argument 2, they
dnl   are #included in the search. The header files are only included in the search if they
dnl   have already been found using the AC_CHECK_HEADERS(header) macro.  
dnl Examples:
dnl     AC_CHECK_POINTER_TYPE(pthread_d, pthread.h)
dnl     AC_CHECK_POINTER_TYPE(void *)

dnl AC_CHECK_POINTER_TYPE(FUNCTION, HEADER-FILE...)
AC_DEFUN(AC_CHECK_POINTER_TYPE,
[AC_MSG_CHECKING([ifelse([$2], , [if $1 is a pointer type], [if $1 is a pointer type (in $2)])])
AH_TEMPLATE(AS_TR_CPP(HAVE_POINTER_TYPE_$1), [Define if $1 is a pointer type on your system])
ifelse([$2], , [ac_includes=""
],
[ac_includes=""
for ac_header in $2
do
  ac_safe=`echo "$ac_header" | sed 'y%./+-%__p_%'`
  if eval "test \"`echo '$''{'ac_cv_header_$ac_safe'}'`\" = yes"; then
    ac_includes="$ac_includes 
#include<$ac_header>"
  fi
done])
tmp_save_1=`echo $1 | tr ' :' '__'`
AC_CACHE_VAL(ac_cv_pointer_type_$tmp_save_1,
[AC_TRY_COMPILE(
[#ifdef __cplusplus
extern "C" {
#endif
$ac_includes
#ifdef __cplusplus
}
#endif
],[$1 p; unsigned long l = p],
eval "ac_cv_pointer_type_$tmp_save_1=no", eval "ac_cv_pointer_type_$tmp_save_1=yes")])dnl
if eval "test \"`echo '$''{'ac_cv_pointer_type_$tmp_save_1'}'`\" = yes"; then
  AC_MSG_RESULT(yes)
changequote(, )dnl
  ac_tr_prototype=HAVE_POINTER_TYPE_`echo $tmp_save_1 | tr '[a-z]' '[A-Z]'`
changequote([, ])dnl
  AC_DEFINE_UNQUOTED([$ac_tr_prototype])
  ifelse([$3], , :, [$3])
else
  AC_MSG_RESULT(no)
fi
])


dnl
dnl CHECK_VLA checks if the C Compiler supports variable-length arrays
dnl
AC_DEFUN(CHECK_VLA,
[AC_MSG_CHECKING([whether variable-length arrays are supported])
AH_TEMPLATE([HAVE_VLA], [Define if variable-length arrays are supported in C])
AC_LANG_SAVE
AC_LANG_C
AC_TRY_COMPILE([], [
 int n; int foo[n];
],
[AC_MSG_RESULT(yes); AC_DEFINE(HAVE_VLA)],
AC_MSG_RESULT(no))
AC_LANG_RESTORE
])


#    AC_TYPEDEF_HELPER(TYPE,
#                  [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND],
#                  [INCLUDES])

m4_define([AC_TYPEDEF_HELPER],
[AS_VAR_PUSHDEF([ac_Type], [ac_cv_type_$1])dnl
AC_CACHE_CHECK([for $1], ac_Type,
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([AC_INCLUDES_DEFAULT([$4])],
[if (($1 *) 0)
  return 0;
if (sizeof ($1))
  return 0;])],
                   [AS_VAR_SET(ac_Type, yes)],
                   [AS_VAR_SET(ac_Type, no)])])
AS_IF([test AS_VAR_GET(ac_Type) = yes], [$2], [$3])[]dnl
AS_VAR_POPDEF([ac_Type])dnl
])# AC_TYPEDEF_HELPER

AC_DEFUN(AC_TYPEDEF_HELPER2,[
AH_VERBATIM([$3], [/* Define `$1' to `$2' if <sys/types.h> does not define. */
#undef $3
#ifdef $3
typedef $2 $1;
#endif])])# AC_TYPEDEF_HELPER2

#
#  AC_TYPEDEF(type, default)
#
#  This macro works similar to the old (deprecated, pre-2.13) AC_CHECK_TYPE
#  macro, but instead of creating a #define for the type if absent, it
#  creates a clean typedef.
#
AC_DEFUN(AC_TYPEDEF,[
m4_define(AC_TYPEDEF_TEMP,[AS_TR_CPP(HAVE_NO_TYPEDEF_$1)])
AC_TYPEDEF_HELPER2([$1],[$2],AC_TYPEDEF_TEMP)
AC_TYPEDEF_HELPER([$1],[],[AC_DEFINE_UNQUOTED(AC_TYPEDEF_TEMP)])
])# AC_TYPEDEF

dnl
dnl $Log: aclocal.m4,v $
dnl Revision 1.37  2005/11/15 16:05:51  meichel
dnl Configure test AC_CHECK_INTP_SELECT now only includes header files
dnl   that have been successfully tested for presence before.
dnl
dnl Revision 1.36  2005/11/07 11:23:34  meichel
dnl Replaced old AC_CHECK_TYPE macros by newly developed AC_TYPDEF macro.
dnl   Missing types are now declared by type definition and not as macros anymore.
dnl
dnl Revision 1.35  2004/10/20 15:54:01  meichel
dnl Added configure tests for <stdbool.h> and a number of C typedefs,
dnl   needed for JasPer support.
dnl
dnl Revision 1.34  2004/08/04 14:50:45  meichel
dnl Fixed configure test that checks if pthread_t is a pointer type to
dnl   correctly work on compilers such as gcc 2.95 where assignment from
dnl   int to void pointer does not cause a compiler error.
dnl
dnl Revision 1.33  2004/08/03 16:28:40  meichel
dnl Added configure test to check if pthread_t is a pointer type
dnl
dnl Revision 1.32  2004/08/03 11:29:04  meichel
dnl Added configure test that checks if <libc.h> needs to be treated as a C++
dnl   header, i.e. included without extern "C". Needed for QNX.
dnl
dnl Revision 1.31  2004/01/21 11:57:56  meichel
dnl Fixed AC_CHECK_PROTOTYPE autoconf macro to support names containing
dnl   space or colon characters.
dnl
dnl Revision 1.30  2003/12/17 17:36:18  meichel
dnl Added configure test that checks if libtiff supports LZW compression
dnl
dnl Revision 1.29  2003/12/11 13:38:57  meichel
dnl Added configure tests for <new.h> and std::nothrow
dnl
dnl Revision 1.28  2003/12/10 13:29:54  meichel
dnl Re-worked configure scripts for Autoconf 2.5x
dnl   Presence of external library is now checked automatically.
dnl
dnl Revision 1.27  2003/07/09 12:22:46  meichel
dnl Added configure test for new-style cast operators such as
dnl   static_cast<> and const_cast<>.
dnl
dnl Revision 1.26  2003/07/03 15:00:55  meichel
dnl Added configure test for "typename" C++ keyword
dnl
dnl Revision 1.25  2003/07/03 14:49:05  meichel
dnl Fixed AC_CHECK_DECLARATION macro
dnl
dnl Revision 1.24  2003/06/06 10:23:41  meichel
dnl Added configure tests for bool and volatile keywords
dnl
dnl Revision 1.23  2003/05/13 09:55:30  meichel
dnl Fixed minor issue in AC_CHECK_INTP_SELECT configure macro
dnl
dnl Revision 1.22  2002/12/16 16:19:25  meichel
dnl Added configure test that checks if extern "C" inclusion
dnl   of <math.h> fails, e.g. on HP/UX 10 and WIN32
dnl
dnl Revision 1.21  2002/12/16 11:00:57  meichel
dnl Added configure test that checks if signal handler functions
dnl   require ellipse (...) parameters, for example on Irix5.
dnl
dnl Revision 1.20  2002/12/11 13:08:28  meichel
dnl Added configure test for type of 5th parameter of getsockopt()
dnl
dnl Revision 1.19  2002/05/15 14:13:11  meichel
dnl Added configure test for readdir_r conforming to Posix 1.c draft 6
dnl
dnl Revision 1.18  2002/04/16 14:06:18  joergr
dnl Added configurable support for C++ ANSI standard includes (e.g. streams).
dnl Thanks to Andreas Barth <Andreas.Barth@bruker-biospin.de> for his
dnl contribution.
dnl
dnl Revision 1.17  2001/12/18 09:51:57  meichel
dnl Modified configure test for "const" support of the C compiler
dnl   in order to avoid a macro recursion error on Sun CC 2.0.1
dnl
dnl Revision 1.16  2001/11/02 12:03:42  meichel
dnl Added new configure tests for std::_Ios_Openmode and ios::nocreate,
dnl   required for gcc 3.0.x.
dnl
dnl Revision 1.15  2001/08/23 16:29:11  meichel
dnl Added configure tests required by dcmjpeg module
dnl
dnl Revision 1.14  2000/12/20 09:54:29  meichel
dnl Fixed remaining problems with configure on FreeBSD.
dnl
dnl Revision 1.13  2000/12/19 12:15:45  meichel
dnl Updated configure for the FreeBSD Posix implementation which requires
dnl   a special gcc option -pthread to cause linking with libc_r instead of libc.
dnl
dnl Revision 1.12  2000/09/08 14:20:11  meichel
dnl Added new options to configure
dnl
dnl Revision 1.11  2000/09/05 12:19:32  joergr
dnl Added new test checking for the presence of type ssize_t.
dnl
dnl Revision 1.10  2000/03/10 11:55:43  meichel
dnl Added special configure test for <netinet/in.h> and <netinet/tcp.h>,
dnl   needed for IRIX 6.
dnl
dnl Revision 1.9  2000/02/24 13:51:52  meichel
dnl Added new check that distinguishes NeXT's libtool from GNU libtool (which
dnl   has a totally different purpose). Required because the old configure scheme
dnl   failed if GNU libtool was found in the search path.
dnl
dnl Revision 1.8  1999/04/28 16:49:47  meichel
dnl Added test whether the compiler supports the new explicit template
dnl   specialization syntax, e.g. template<> int a_class<int>::a_method()
dnl
dnl Revision 1.7  1997/09/11 15:53:17  hewett
dnl Enhanced the configure macro AC_CHECK_PROTOTYPE to check the
dnl include files passed as aruments before searching for a
dnl prototype.  This makes the configure.in file considerably
dnl simpler.  The include files passed as aruments to the
dnl AC_CHECL_PROTOTYPE macro must have already been tested for
dnl using the AC_CHECK_HEADERS macro.  If not, the include files
dnl are assumed not to exist.
dnl
dnl Revision 1.6  1997/07/03 09:38:17  meichel
dnl Corrected bug in configure module - all tests trying to link or run
dnl   a test program could fail (and, therefore, report an incorrect result)
dnl   if libg++.a was not found in the default link path.
dnl
dnl Revision 1.5  1997/07/02 11:53:02  andreas
dnl - Preliminary release of the OFFIS Standard Library.
dnl   In the future this library shall contain a subset of the
dnl   ANSI C++ Library (Version 3) that works on a lot of different
dnl   compilers. Additionally this library shall include classes and
dnl   functions that are often used. All classes and functions begin
dnl   with OF... This library is independent of the DICOM development and
dnl   shall contain no DICOM specific stuff.
dnl
dnl Revision 1.4  1996/12/03 15:28:19  meichel
dnl Added support for HP-UX 9.05 systems using GCC 2.7.2.1
dnl
dnl Revision 1.3  1996/03/28 11:05:22  meichel
dnl Added macro AC_CHECK_DECLARATION which checks if a type declaration exists
dnl in certain header files (e.g. for struct sembuf)
dnl
dnl Revision 1.2  1995/11/28 15:59:46  meichel
dnl Added macro AC_CHECK_PROTOTYPE which allows to check for
dnl the existance of prototype declarations. Added CVS log.
dnl