/3rdparty

This directory contains third party libraries that may be used within the
project.  For the moment, only one library is present: wxWidgets.

The wxWidgets library present here is the 2.6.1 version compiled for Windows
with the following flags:
CFLAGS:   -march=i386 -mtune=i686 -O2 -fweb -frename-registers
	  -fomit-frame-pointer -pipe
CXXFLAGS: (same as CFLAGS)
CPPFLAGS: (empty)
LDFLAGS:  (empty)

Configure script (./configure) has been invoked with the following arguments:
./configure --disable-shared --enable-no_rtti --enable-no_deps \
	    --disable-compat24 --disable-rpath --disable-sockets \
	    --disable-unicode --disable-threads --disable-html \
	    --disable-xrc --disable-debugreport --disable-postscript \
	    --disable-calendar --disable-datepick --disable-grid \
	    --disable-help --disable-joystick --disable-sash \
	    --disable-sound --disable-splash --disable-tipdlg \
	    --disable-wizarddlg --disable-gif --disable-pnm --disable-pcx \
	    --without-libpng --without-libjpeg --without-libtiff \
	    --without-regex --without-zlib --without-expat
This corresponds to the "base" and "core" modules, minus additional image
filters (only native ICO and CUR, as well as the commonly used XPM). This is
a static build (to get rid of DLL dependencies).

See "../GNUmakefile" for more details.  To compile it yourself, refer to
"../README.txt" (the section about wxWidgets (cross-)compilation).

Note: wxmsw/bin/wx-config.sh is here for reference only; paths (temporary
place) and libraries names (not containing platform name any more) have
changed.  Do NOT rely on this file!
