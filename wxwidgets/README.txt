/wxwidgets

This directory contains a precompiled Win32 version of wxWidgets 2.6.0.

It has been compiled with the following flags:
CFLAGS:   -march=i386 -mtune=i686 -O2 -fweb -frename-registers
	  -fomit-frame-pointer -pipe
CXXFLAGS: CFLAGS + -fno-check-new
CPPFLAGS: (empty)
LDFLAGS:  (empty)

Configure script (./configure) has been invoked with the following arguments:
./configure --enable-static --disable-shared --disable-debug \
	    --enable-unicode --disable-threads --disable-compat24
