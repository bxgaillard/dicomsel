#!/bin/sh

if [ $# -lt 2 -o $# -gt 3 ]; then
    echo "Syntax: $0 <srcdir> <dstdir> [module1,module2...]"
    exit 1
fi

srcdir="$1"
dstdir="$2"
modules=",$3,"

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

exec 3>&- 4>&-

init_am='AM_INIT_AUTOMAKE([1.7 no-define dist-bzip2 -Wall])'

sed -e 's/\[\(dcmtk\)-[0-9]\+\.[0-9]\+\.[0-9]\+\]/[\1]/' \
    -e 's/Makefile\.in/Makefile.am/' \
    -e 's/^AC_CONFIG_AUX_DIR([^)]\+)$/'"$init_am/" \
    -e 's/^\(AC_CONFIG_HEADER([^)]\+)\)$/\1\nAC_CONFIG_AUX_DIR(autotools)/' \
    -e 's/^\(AC_PROG_CC\)$/\1\nAM_PROG_CC_C_O/' \
    -e 's/^AC_CHECK_PROGS(LIBTOOL[^)]\+)$//' \
    -e 's/^AC_OUTPUT([^)]\+)$/AC_OUTPUT(Makefile)/' \
    <"$srcdir/config/configure.in" >"$dstdir/configure.ac"
cp -p "$srcdir/config/aclocal.m4" "$dstdir/aclocal-2.m4"
cp -p "$srcdir/HISTORY" "$dstdir/NEWS"
cp -p "$srcdir/COPYRIGHT" "$dstdir/COPYING"
cp -p "$srcdir/CHANGES".??? "$dstdir/ChangeLog"
cp -p "$srcdir/README" "$srcdir/INSTALL" "$dstdir"
echo 'See COPYING.' >"$dstdir/AUTHORS"
mkdir "$dstdir/autotools"

cd "$dstdir"
aclocal
cat aclocal-2.m4 >> aclocal.m4
rm -f aclocal-2.m4
autoconf
autoheader
automake -a -c
rm -rf autom4te.cache
