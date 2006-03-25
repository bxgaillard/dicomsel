#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Syntax: $0 <srcdir> <dstdir>"
    exit 1
fi

srcdir="$1"
dstdir="$2"

rm -rf "$dstdir"
mkdir "$dstdir"
mkdir "$dstdir/include"
mkdir "$dstdir/include/dcmtk"

exec 3>"$dstdir/Makefile.am"

echo 'AUTOMAKE_OPTIONS = subdir-objects' >&3
echo >&3
echo -n 'AM_CPPFLAGS = -I$(srcdir)/include' >&3
for sdir in "$srcdir"/dcmjpeg/libijg*; do
    echo ' \' >&3
    echo -n "    -I\$(srcdir)/dcmjpeg/`basename "$sdir"`" >&3
done
echo >&3
echo >&3
echo -n 'noinst_LIBRARIES =' >&3

for dir in "$srcdir"/*/libsrc; do
    basedir="`dirname "$dir"`"
    echo ' \' >&3
    echo -n "    lib`basename "$basedir"`.a" >&3
done
echo >&3

for dir in "$srcdir"/*/libsrc; do
    basedir="`dirname "$dir"`"
    basedir="`basename "$basedir"`"

    mkdir "$dstdir/$basedir"
    cp -p "$dir"/*.cc "$dstdir/$basedir"

    echo >&3
    echo -n "lib$basedir"'_a_SOURCES =' >&3

    for src in "$dir"/*.cc; do
	echo ' \' >&3
	echo -n "    $basedir/`basename "$src"`" >&3
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

	    for src in "$sdir"/*.c "$sdir"/*.h; do
		echo ' \' >&3
		echo -n "    dcmjpeg/$basesdir/`basename "$src"`" >&3
	    done
	done
    fi

    echo >&3
done

echo >&3
echo -n 'EXTRA_DIST =' >&3

for dir in "$srcdir"/*/include; do
    basedir="`dirname "$dir"`"
    basedir="`basename "$basedir"`"

    mkdir "$dstdir/include/dcmtk/$basedir"
    cp -pr "$dir/dcmtk/$basedir"/*.h "$dstdir/include/dcmtk/$basedir"

    if [ ! -d "$srcdir/$basedir/libsrc" ]; then
	for src in "$dir/dcmtk/$basedir"/*.h; do
	    echo ' \' >&3
	    echo -n "    include/dcmtk/$basedir/`basename "$src"`" >&3
	done
    fi
done
echo >&3

exec 3>&-

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
