#!/bin/sh

if [ $# -lt 2 -o $# -gt 3 ]; then
    echo "Syntax: $0 <src.tar.gz> <dstdir> [module1,module2...]" 1>&2
    exit 1
fi

srcarc="$1"
dstdir="$2"

if [ ! -f "$srcarc" ]; then
    echo "Fatal: '$srcarc' doesn't exist." 1>&2
    exit 2
fi

if [ ! -d "$dstdir" ]; then
    echo "Fatal: '$dstdir' doesn't exist." 1>&2
    exit 3
fi

case "$srcarc" in
    *.tar.gz | *.tgz )
	arcopt=z
	;;
    *.tar.bz2 )
	arcopt=j
	;;
    * )
	echo 'Fatal: unknown archive format.' 1>&2
	exit 4
esac

srcver="`basename "$srcarc" | \
	 sed 's/.*-\(.*\)\.\(tar\.\(gz\|bz2\)\|tgz\)/\1/'`"

echo 'Extracting archive...'
tmpdir="`mktemp -d /tmp/dcmtk.XXXXXXXXXX`"
tar -x"$arcopt"f "$1" -C "$tmpdir"
srcdir="$tmpdir/dcmtk-$srcver"
dstdir="$dstdir/dcmtk-$srcver"
modules=",$3,"

echo 'Copying files...'

rm -rf "$dstdir"
mkdir "$dstdir"
mkdir "$dstdir/include"
mkdir "$dstdir/include/dcmtk"

exec 3>"$dstdir/Makefile.am" 4>"$dstdir/Makefile.msc"

echo 'AUTOMAKE_OPTIONS = subdir-objects' >&3
echo >&3

echo '.SUFFIXES:' >&4
echo '.SUFFIXES: .c .cc .obj' >&4
echo '.c.obj: ; cl /nologo /MT /TC /Ox /DNDEBUG $(CPPFLAGS) /c $< /Fo$@' >&4
echo -n '.cc.obj: ; cl /nologo /MT /TP /Ox /EHsc /DNDEBUG $(CPPFLAGS) ' >&4
echo '/c $< /Fo$@' >&4
echo >&4

echo 'AM_CPPFLAGS = \' >&3
echo -n '    -I$(srcdir)/include' >&3
echo 'CPPFLAGS = \' >&4
echo -n '    /Iinclude' >&4

case "$modules" in
    ,, | *,dcmjpeg,* )
	for sdir in "$srcdir"/dcmjpeg/libijg*; do
	    basedir="`basename "$sdir"`"

	    echo ' \' >&3
	    echo -n "    -I\$(srcdir)/dcmjpeg/$basedir" >&3
	    echo ' \' >&4
	    echo -n "    /Idcmjpeg\\$basedir" >&4
	done
    ;;
esac

echo >&3
echo >&3
echo -n 'noinst_LIBRARIES =' >&3
echo >&4
echo >&4
echo -n 'LIBRARIES =' >&4

for dir in "$srcdir"/*/libsrc; do
    basedir="`dirname "$dir"`"
    basedir="`basename "$basedir"`"

    echo ' \' >&3
    echo -n "    lib$basedir.a" >&3
    echo ' \' >&4
    echo -n "    $basedir.lib" >&4
done
echo >&3
echo >&4

echo >&4
echo 'all: $(LIBRARIES)' >&4
echo '$(LIBRARIES): ; link /LIB /NOLOGO /OUT:$@ $**' >&4

for dir in "$srcdir"/*/libsrc; do
    basedir="`dirname "$dir"`"
    basedir="`basename "$basedir"`"

    case "$modules" in
	,, | *,"$basedir",* )
	    mkdir "$dstdir/$basedir"
	    cp -p "$dir"/*.cc "$dstdir/$basedir"

	    echo >&3
	    echo -n "lib$basedir"'_a_SOURCES =' >&3
	    echo >&4
	    echo -n "$basedir.lib:" >&4

	    for src in "$dir"/*.cc; do
		echo ' \' >&3
		echo -n "    $basedir/`basename "$src"`" >&3
		echo ' \' >&4
		echo -n "    $basedir\\`basename "$src" .cc`.obj" >&4
	    done
	    if [ "x`echo "$dir"/*.h`" != "x$dir/*.h" ]; then
		cp -p "$dir"/*.h "$dstdir/$basedir"
		for src in "$dir"/*.h; do
		    echo ' \' >&3
		    echo -n "    $basedir/`basename "$src"`" >&3
		done
	    fi
	    for src in "$srcdir/$basedir/include/dcmtk/$basedir"/*.h; do
		echo ' \' >&3
		echo -n "    include/dcmtk/$basedir/`basename "$src"`" >&3
	    done

	    if [ "x$basedir" = xdcmjpeg ]; then
		for sdir in "$srcdir"/dcmjpeg/libijg*; do
		    basesdir="`basename "$sdir"`"

		    mkdir "$dstdir/dcmjpeg/$basesdir"
		    cp -p "$sdir"/*.c "$sdir"/*.h "$dstdir/dcmjpeg/$basesdir"

		    for src in "$sdir"/*.c; do
			echo ' \' >&3
			echo -n "    dcmjpeg/$basesdir/`basename "$src"`" >&3
			echo ' \' >&4
			echo -n "    dcmjpeg\\$basesdir\\" >&4
			echo -n "`basename "$src" .c`.obj" >&4
		    done
		    for src in "$sdir"/*.h; do
			echo ' \' >&3
			echo -n "    dcmjpeg/$basesdir/`basename "$src"`" >&3
		    done
		done
	    fi

	    echo >&3
	    echo >&4
	;;
    esac
done

echo >&3
echo 'EXTRA_DIST = \' >&3
echo -n '    Makefile.msc' >&3

for dir in "$srcdir"/*/include; do
    basedir="`dirname "$dir"`"
    basedir="`basename "$basedir"`"

    case "$modules" in
	,, | *,"$basedir",* )
	    mkdir "$dstdir/include/dcmtk/$basedir"
	    cp -pr "$dir/dcmtk/$basedir"/*.h "$dstdir/include/dcmtk/$basedir"

	    if [ ! -d "$srcdir/$basedir/libsrc" ]; then
		for src in "$dir/dcmtk/$basedir"/*.h; do
		    echo ' \' >&3
		    echo -n "    include/dcmtk/$basedir/`basename "$src"`" >&3
		done
	    fi
	;;
    esac
done
echo >&3

echo >&4
echo 'clean:'>&4

for dir in "$srcdir"/*/libsrc; do
    basedir="`dirname "$dir"`"
    basedir="`basename "$basedir"`"

    case "$modules" in
	,, | *,"$basedir",* )
	    for src in "$dir"/*.cc; do
		fname="$basedir\\`basename "$src" .cc`.obj"
		echo $'\t'"if exist $fname del $fname" >&4
	    done

	    if [ "x$basedir" = xdcmjpeg ]; then
		for sdir in "$srcdir"/dcmjpeg/libijg*; do
		    for src in "$sdir"/*.c; do
			fname="dcmjpeg\\`basename "$sdir"`\\"
			fname="$fname`basename "$src" .c`.obj"
			echo $'\t'"if exist $fname del $fname" >&4
		    done
		done
	    fi

	    fname="$basedir.lib"
	    echo $'\t'"if exist $fname del $fname" >&4
	;;
    esac
done

exec 3>&- 4>&-

init_am='AM_INIT_AUTOMAKE([1.7 no-define dist-bzip2 -Wall])'
def_pth='AC_DEFINE(\[HAVE_POINTER_TYPE_PTHREAD_T\], \[1\],'
def_pth="$def_pth \\[Wether pthread_t is a pointer\\])"

sed -e 's/\[\(dcmtk\)-[0-9]\+\.[0-9]\+\.[0-9]\+\]/[\1]/' \
    -e 's/Makefile\.in/Makefile.am/' \
    -e 's/^AC_CONFIG_AUX_DIR([^)]\+)$/'"$init_am/" \
    -e 's/^\(AC_CONFIG_HEADER([^)]\+)\)$/\1\nAC_CONFIG_AUX_DIR(autotools)/' \
    -e 's/^\(AC_PROG_CC\)$/\1\nAM_PROG_CC_C_O/' \
    -e 's/^AC_CHECK_PROGS(LIBTOOL[^)]\+)$//' \
    -e 's/^AC_OUTPUT([^)]\+)$/AC_OUTPUT(Makefile)/' \
    <"$srcdir/config/configure.in" >"$dstdir/configure.ac"
cp -p "$srcdir/HISTORY" "$dstdir/NEWS"
cp -p "$srcdir/COPYRIGHT" "$dstdir/COPYING"
cp -p "$srcdir/CHANGES".??? "$dstdir/ChangeLog"
cp -p "$srcdir/README" "$srcdir/INSTALL" "$dstdir"
echo 'See COPYING.' >"$dstdir/AUTHORS"

mkdir "$dstdir/autotools"
cp -p "$srcdir/config/aclocal.m4" "$dstdir/autotools/dcmtk.m4"
rm -rf "$tmpdir"

echo 'Patching files...'
cd "$dstdir"
patch -Np1 << EOF
diff -ur dcmtk-3.5.4.old/autotools/dcmtk.m4 dcmtk-3.5.4/autotools/dcmtk.m4
--- dcmtk-3.5.4.old/autotools/dcmtk.m4	2005-11-15 17:05:51 +0100
+++ dcmtk-3.5.4/autotools/dcmtk.m4	2006-11-17 08:27:38 +0100
@@ -1408,7 +1408,14 @@
 #ifdef __cplusplus
 }
 #endif
-],[\$1 p; unsigned long l = p],
+],[
+\$1 p; unsigned long l =
+#ifdef __cplusplus
+static_cast<unsigned long>(p)
+#else
+p
+#endif
+],
 eval "ac_cv_pointer_type_\$tmp_save_1=no", eval "ac_cv_pointer_type_\$tmp_save_1=yes")])dnl
 if eval "test \\"\`echo '\$''{'ac_cv_pointer_type_\$tmp_save_1'}'\`\\" = yes"; then
   AC_MSG_RESULT(yes)
diff -ur dcmtk-3.5.4.old/dcmdata/mkdeftag.cc dcmtk-3.5.4/dcmdata/mkdeftag.cc
--- dcmtk-3.5.4.old/dcmdata/mkdeftag.cc	2005-12-09 16:04:37 +0100
+++ dcmtk-3.5.4/dcmdata/mkdeftag.cc	2006-08-05 02:05:54 +0200
@@ -154,9 +154,11 @@
 static char*
 getUserName(char* userString, int maxLen)
 {
-#if defined(_REENTRANT) && !defined(_WIN32) && !defined(__CYGWIN__)
+#if defined(_REENTRANT) && !defined(_WIN32) && !defined(__CYGWIN__) \\
+    && defined(_POSIX_THREAD_SAFE_FUNCTIONS)
     // use getlogin_r instead of getlogin
-    return getlogin_r(userString, int maxLen)
+    getlogin_r(userString, maxLen);
+    return userString;
 #else
     char* s;
     s = getlogin(); // thread unsafe
diff -ur dcmtk-3.5.4.old/dcmdata/mkdictbi.cc dcmtk-3.5.4/dcmdata/mkdictbi.cc
--- dcmtk-3.5.4.old/dcmdata/mkdictbi.cc	2005-12-09 16:04:37 +0100
+++ dcmtk-3.5.4/dcmdata/mkdictbi.cc	2006-08-05 02:06:43 +0200
@@ -137,9 +137,11 @@
 static char*
 getUserName(char* userString, int maxLen)
 {
-#if defined(_REENTRANT) && !defined(_WIN32) && !defined(__CYGWIN__)
+#if defined(_REENTRANT) && !defined(_WIN32) && !defined(__CYGWIN__) \\
+    && defined(_POSIX_THREAD_SAFE_FUNCTIONS)
     // use getlogin_r instead of getlogin
-    return getlogin_r(userString, int maxLen)
+    getlogin_r(userString, maxLen);
+    return userString;
 #else
     char* s;
     s = getlogin(); // thread unsafe
EOF

echo 'Running Autotools...'
aclocal-1.10 -I autotools || aclocal -I autotools
autoconf-2.60 || autoconf
autoheader-2.60 || autoheader
automake-1.10 -a -c || automake -a -c
rm -rf autom4te.cache

echo 'Done.'
