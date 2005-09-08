#!/bin/bash

# wxWidgets for Windows Cross-Builder
# Compile wxWidgets for Win32 under an UNIX environment using the
# MinGW cross-compiler

ARCH=i386-mingw32msvc
TMP="/tmp/$(basename "$0" .sh | tr -d '\n' | tr ' \t' __)"
BUILD="$TMP/build"
DEST=/tmp/wxmsw


escape() {
    local APO="'\\''"
    echo "'${1//"'"/$APO}'"
}

TAB=$'\t'
SPACES="${0//?/ }    "
SPACES="${SPACES//        /$TAB}"
USAGE="Usage:
    $0 <source-archive>
${SPACES}[-a|--arch architecture]
${SPACES}[-d|--dest destination-directory]
${SPACES}[-h|--help]
${SPACES}[<configure-option>...]

With:
    architecture:          target architecture (default: \"$ARCH\")
    destination-directory: build destination (default: \"$DEST\")
    configure-option:      --{enable,disable,with,without}-*

The destination directory will contain:
    destdir/
     |-- bin/
     |    |-- wx-config.sh
     |-- include/
     |    |-- wx/
     |         |-- *.h
     |-- lib/
          |-- libwx*.a

To use the builded library, use the following flags:
    CPPFLAGS: -Idestdir/include
    LDFLAGS:  -Ldestdir/lib -llib1 -llib2 -llib3...
Some other flags may be needed depending on which options you enabled or
disabled with the \"configure\" script.  \"destdir/bin/wx-config\" may help,
but beware of the directory changes."
unset TAB SPACES

# Verify parameter presence
if [ $# -eq 0 ]; then
    echo "$USAGE" >&2
    exit 1
fi

# Parse parameters
unset SRC CONFIG_OPT
while [ $# -gt 0 ]; do
    case "$1" in
	--enable-* | --disable-* | --with-* | --without-*)
	    CONFIG_OPT="$CONFIG_OPT $(escape "$1")"
	    ;;

	-a | --arch)
	    if [ $# -eq 1 ]; then
		echo "Error: \"$1\" needs an argument." >&2
		exit 3
	    fi
	    ARCH="$2"
	    shift
	    ;;

	-d | --dest)
	    if [ $# -eq 1 ]; then
		echo "Error: \"$1\" needs an argument." >&2
		exit 3
	    fi
	    DEST="$2"
	    shift
	    ;;

	-h | --help)
	    echo "$USAGE"
	    exit 0
	    ;;

	--)
	    shift
	    break
	    ;;

	-*)
	    echo "Error: unknown option \"$1\"." >&2
	    exit 3
	    ;;

	*)
	    if [ -n "$SRC" ]; then
		echo 'Error: more than one source archive specified.' >&2
		exit 2
	    fi
	    SRC="$1"
	    ;;
    esac
    shift
done
unset USAGE

# Parse remaining parameters (when "--" is used)
if [ $# -gt 0 ]; then
    if [ -n "$SRC" -o $# -gt 1 ]; then
	echo 'Error: more than one source archive specified.' >&2
	exit 2
    fi
    SRC="$1"
fi

# Remove space at the beginning of $CONFIG_OPT
CONFIG_OPT="${CONFIG_OPT:+${CONFIG_OPT:1}}"

# Verify archive presence and readability
if [ -z "$SRC" ]; then
    echo 'Error: no source archive specified.' >&2
    exit 2
fi
if [ ! -f "$SRC" -o ! -r "$SRC" ]; then
    echo "Error: cannot read source archive \"$SRC\"." >&2
    exit 4
fi

# Determine archive format and corresponding command
case "$SRC" in
    *.tar.gz | *.tgz)
	CMD="tar xfz"
	;;

    *.tar.bz2)
	CMD="tar xfj"
	;;

    *.tar)
	CMD="tar xf"
	;;

    *.zip)
	CMD="unzip"
	;;

    *)
	echo "Error: unknown archive format for file \"$SRC\"." >&2
	exit 4
	;;
esac

# Determine sources version
VERSION="$(echo "$SRC" | sed -e 's/^.*-\([0-9]\+\.[0-9]\+\.[0-9]\+\)\.'\
'\(tar\(\.gz\|\.bz2\)\?\|tgz\|zip\)$/\1/')"
if [ "$VERSION" = "$SRC" ]; then
    echo 'Error: unable to determine the version of the archive.' >&2
    exit 4
fi

# Check compiler suite
"${ARCH:+$ARCH-}g++" --version &>/dev/null || {
    {
	echo "Unable to find C++ compiler \"$GPP\"."
	echo 'You may need to install a cross-compiling suite.'
	echo "If building on Windows, try the \"-a ''\" option."
    } >&2
    exit 5
}

rm -rf "$TMP"
[ -d "$TMP" ] || mkdir "$TMP" || {
    echo "Unable to create directory \"$TMP\"." >&2
    exit 6
}
[ -d "$DEST" ] || mkdir "$DEST" || {
    echo "Unable to create directory \"$DEST\"." >&2
    exit 7
}
[ -d "$BUILD" ] || mkdir "$BUILD" || {
    echo "Unable to create directory \"$DEST\"." >&2
    exit 7
}

{
    echo "Extracting \"$SRC\"..."
    OLD_DIR="$PWD"
    cd "$TMP" &&
    $CMD "$SRC"
} || {
    echo 'Unable to extract archive.' >&2
    exit 9
}
unset CMD

CONFIGURE="./configure --prefix=$(escape "$BUILD") \
${ARCH:+--host=$(escape "$ARCH") --target=$(escape "$ARCH")} \
CFLAGS='-march=i386 -mtune=i686 -O2 -fweb -frename-registers \
-fomit-frame-pointer -pipe' \
CXXFLAGS='-march=i386 -mtune=i686 -O2 -fweb -frename-registers \
-fomit-frame-pointer -pipe' \
--with-msw $CONFIG_OPT"
unset CC CXX CPP CFLAGS CXXFLAGS CPPFLAGS LDFLAGS
unset MAKEFLAGS MAKELEVEL MFLAGS
{
    cd wxWidgets-$VERSION &&
    echo "Running \"configure\": $CONFIGURE" &&
    eval "$CONFIGURE" &&
    echo 'Now compiling...' &&
    { gmake || make; } &&
    echo 'Now installing...' &&
    { gmake install || make install; }
} || {
    {
	echo 'Error while trying to build and compile wxWidgets.'
	echo "Maybe aren't you using the correct archive?"
	echo 'Check errors above.'
	echo "WARNING: files in \"$TMP\" aren't removed so you can make" \
	     checkings.
    } >&2
    exit 10
}

echo "Copying files into \"$DEST\"..."
{
    # $DEST/bin
    cd "$OLD_DIR" &&
    { [ -d "$DEST/bin" ] || mkdir "$DEST/bin"; } &&
    cp -f "$BUILD/bin/wx-config" "$DEST/bin/wx-config.sh" &&
    { [ ! -f "$BUILD/bin/wxrc.exe" ] ||
	cp -f "$BUILD/bin/wxrc.exe" "$DEST/bin"; } &&

    # $DEST/include
    { [ -d "$DEST/include" ] || mkdir "$DEST/include"; } &&
    cp -rf "$BUILD/include/wx-"*/wx "$DEST/include" &&
    cp -f "$BUILD/lib/wx/include/"*/wx/setup.h "$DEST/include/wx" &&

    # $DEST/lib
    { [ -d "$DEST/lib" ] || mkdir "$DEST/lib"; } && {
	if [ -n "$ARCH" ]; then
	    unset stop
	    for lib in "$BUILD/lib/libwx"*.a; do
		dst="${lib#"$BUILD/lib/"}"
		cp -f "$lib" "$DEST/lib/${dst%"-$ARCH.a"}.a" || {
		    stop=y
		    break
		}
	    done
	    unset lib dst
	    [ -z "$stop" ]
	else
	    cp -f "$BUILD/lib/libwx"*.a "$DEST/lib"
	fi
    }
} || {
    {
	echo "Error while copying files in \"$DEST\"."
	echo "WARNING: files in \"$TMP\" aren't removed so you can make" \
	     checkings.
    } >&2
    exit 11
}

echo 'Removing temporary files...'
rm -rf "$TMP" &>/dev/null

echo 'Everything done. Enjoy!'
exit 0
