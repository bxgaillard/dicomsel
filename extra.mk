# ----------------------------------------------------------------------------
#
# DicomSel: DICOM Image Selector
#
# Copyright (C) 2005, 2006 IRCAD
# Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
#
# ----------------------------------------------------------------------------
#
# << LICENSE >>
#
# ----------------------------------------------------------------------------


RM = rm -f
CP = cp -fp
MV = mv -f

WINE = env -i WINEDEBUG=-all wine

MACX86CC  = gcc-4.0 -arch i386
MACPPCCC  = gcc-3.3 -arch ppc
MACX86CXX = g++-4.0 -arch i386
MACPPCCXX = g++-3.3 -arch ppc

CFLAGS   = -O2 -fomit-frame-pointer -pipe
CXXFLAGS = $(CFLAGS) -fno-rtti
X86FLAGS = -march=i386 -mtune=i686

MACX86CFLAGS   = -march=prescott -mfpmath=sse -mmacosx-version-min=10.4
MACX86CPPFLAGS = -DMAC_OS_X_VERSION_MIN_REQUIRED=1040 -nostdinc \
		 -F/Developer/SDKs/MacOSX10.4u.sdk/System/Library/Frameworks \
		 -I/Developer/SDKs/MacOSX10.4u.sdk/usr/lib/gcc/i686-apple-darwin8/4.0.1/include \
		 -I/Developer/SDKs/MacOSX10.4u.sdk/usr/include/c++/4.0.0 \
		 -I/Developer/SDKs/MacOSX10.4u.sdk/usr/include/c++/4.0.0/i686-apple-darwin8 \
		 -isystem /Developer/SDKs/MacOSX10.4u.sdk/usr/include
MACX86LDFLAGS  = -mmacosx-version-min=10.4 \
		 -F/Developer/SDKs/MacOSX10.4u.sdk/System/Library/Frameworks \
		 -L/Developer/SDKs/MacOSX10.4u.sdk/usr/lib/gcc/i686-apple-darwin8/4.0.0 \
		 -Wl,-syslibroot,/Developer/SDKs/MacOSX10.4u.sdk

MACPPCCFLAGS   = -mcpu=750 -mtune=7450
MACPPCCPPFLAGS = -DMAC_OS_X_VERSION_MIN_REQUIRED=1020 -nostdinc \
		 -F/Developer/SDKs/MacOSX10.2.8.sdk/System/Library/Frameworks \
		 -I/Developer/SDKs/MacOSX10.2.8.sdk/usr/include/gcc/darwin/3.3 \
		 -I/Developer/SDKs/MacOSX10.2.8.sdk/usr/include/gcc/darwin/3.3/c++ \
		 -I/Developer/SDKs/MacOSX10.2.8.sdk/usr/include/gcc/darwin/3.3/c++/ppc-darwin \
		 -isystem /Developer/SDKs/MacOSX10.2.8.sdk/usr/include
MACPPCLDFLAGS  = -F/Developer/SDKs/MacOSX10.2.8.sdk/System/Library/Frameworks \
		 -L/Developer/SDKs/MacOSX10.2.8.sdk/usr/lib/gcc/darwin/3.3 \
		 -Wl,-syslibroot,/Developer/SDKs/MacOSX10.2.8.sdk

BINDIR = bin
BUILDDIR = build
WXMSW = wxmsw
WXMSWMINGW = $(WXMSW)/mingw
WXMSWMSVC = $(WXMSW)/msvc
WXMAC = wxmac
WXMACX86 = $(WXMAC)/x86
WXMACPPC = $(WXMAC)/ppc

# --disable-threads (after --disable-unicode) removed
# may make the mingw version require an additional dll...
WXCFG_FLAGS = --disable-shared --enable-no_rtti --enable-no_exceptions \
	      --enable-no_deps --disable-compat24 --disable-rpath \
	      --disable-protocol \
	      --disable-html --disable-xrc --disable-debugreport \
	      --disable-postscript --disable-calendar --disable-datepick \
	      --disable-grid --disable-help --disable-joystick \
	      --disable-sash --disable-sound --disable-splash \
	      --disable-tipdlg --disable-wizarddlg --disable-gif \
	      --disable-pnm --disable-pcx --without-libpng --without-libjpeg \
	      --without-libtiff --without-regex --without-zlib --without-expat
WXCFG_MSW = --with-msw --disable-sockets --disable-unicode
WXCFG_MAC = --with-mac --enable-unicode

EMF = -f extra.mk

.PHONY: default all autogen autoclean conf clean head
.PHONY: dist bindist wxsrc bin linux win32 win32-mingw win32-msc mac

default: all
all:
	$(MAKE) $(EMF) dist
	$(MAKE) $(EMF) bindist
	$(MAKE) $(EMF) autoclean

configure: configure.ac
	$(MAKE) $(EMF) autogen

autogen:
	test -d autotools || mkdir autotools
	aclocal-1.10 -I aclocal || aclocal -I aclocal
	autoconf-2.60 || autoconf
	autoheader-2.60 || autoheader
	automake-1.10 -a -c || automake -a -c

autoclean:
	test ! -f Makefile || make distclean
	rm -rf autom4te.cache config.h.in aclocal.m4 configure autotools \
	    `find . -name Makefile.in | grep -v ^\\./3rdparty`

conf:
	./configure CFLAGS="$(X86FLAGS) $(CFLAGS)" \
		    CXXFLAGS="$(X86FLAGS) $(CXXFLAGS)" LDFLAGS="-s"

dist: configure
	./configure
	$(MAKE) dist-bzip2
	$(MAKE) distclean

bin:
	$(MAKE) $(EMF) linux
	$(MAKE) $(EMF) win32

bin.zip: bin
	find $< | LC_ALL=C sort | zip -9 -@ $@

bindist: bin.zip
	rm -rf bin

linux: $(BINDIR)/linux
$(BINDIR)/linux: configure
	test -d $(BINDIR) || mkdir $(BINDIR)
	@echo "===================== Building for Linux ====================="
	./configure CFLAGS="$(X86FLAGS) $(CFLAGS)" \
		    CXXFLAGS="$(X86FLAGS) $(CXXFLAGS)" LDFLAGS="-s"
	$(MAKE)
	test -d $(BINDIR)/linux || mkdir -p $(BINDIR)/linux
	$(CP) src/dicomsel $(BINDIR)/linux
	$(MAKE) distclean

win32: $(BINDIR)/win32
$(BINDIR)/win32:
	@echo "==================== Building for Windows ===================="
	$(MAKE) -f extra.mk win32-msc || $(MAKE) -f extra.mk win32-mingw

win32-mingw: configure $(WXMSWMINGW)
	./configure --host=mingw32 --target=mingw32 \
		    --with-wx-prefix=$(PWD)/$(WXMSWMINGW) \
		    CFLAGS="$(X86FLAGS) $(CFLAGS)" \
		    CXXFLAGS="$(X86FLAGS) $(CXXFLAGS)" \
		    CPPFLAGS="-DLITTLE_ENDIAN=1234 -DBIG_ENDIAN=4321 \
			      -DBYTE_ORDER=1234" LDFLAGS="-s"
	$(MAKE)
	test -d $(BINDIR)/win32 || mkdir -p $(BINDIR)/win32
	$(CP) src/dicomsel.exe $(BINDIR)/win32
	$(MAKE) distclean

win32-msc: msvc/config.h $(WXMSWMSVC)
	$(MAKE) $(EMF) msvc/flags.mak
	$(WINE) nmake /NOLOGO /f Makefile.msc
	test -d $(BINDIR)/win32 || mkdir -p $(BINDIR)/win32
	$(CP) src/dicomsel.exe $(BINDIR)/win32
	$(WINE) nmake /NOLOGO /f Makefile.msc clean

msvc/config.h msvc/flags.mak: msvc/config.h.in msvc/flags.mak.in configure.ac
	$(MAKE) -f extra.mk configure
	./configure
	$(CP) msvc/config.h msvc/config.h.save
	$(CP) msvc/flags.mak msvc/flags.mak.save
	$(MAKE) distclean
	$(MV) msvc/config.h.save msvc/config.h
	$(MV) msvc/flags.mak.save msvc/flags.mak

mac: $(BINDIR)/mac/DicomSel.app
$(BINDIR)/mac/DicomSel.app: configure $(WXMAC)
	@echo "=================== Building for Mac OS X ===================="
	rm -fr $@
	mkdir -p $@
	./configure CC="$(MACX86CC)" CXX="$(MACX86CXX)" \
		    CFLAGS="$(MACX86CFLAGS) $(CFLAGS)" \
		    CXXFLAGS="$(MACX86CFLAGS) $(CXXFLAGS)" \
		    CPPFLAGS="$(MACX86CPPFLAGS)" \
		    LDFLAGS="$(MACX86LDFLAGS)" \
		    --with-wx-config=`pwd`/$(WXMACX86)/bin/wx-config
	$(MAKE)
	$(CP) src/dicomsel $@/dicomsel.x86
	$(MAKE) distclean
	./configure CC="$(MACPPCCC)" CXX="$(MACPPCCXX)" \
		    CFLAGS="$(MACPPCCFLAGS) $(CFLAGS)" \
		    CXXFLAGS="$(MACPPCCFLAGS) $(CXXFLAGS)" \
		    CPPFLAGS="$(MACPPCCPPFLAGS)" \
		    LDFLAGS="$(MACPPCLDFLAGS)" \
		    --with-wx-config=`pwd`/$(WXMACPPC)/bin/wx-config
	$(MAKE)
	$(CP) src/dicomsel $@/dicomsel.ppc
	@echo "Creating Mac OS X .app directories under $(BINDIR)/mac..."
	mkdir $@/Contents
	$(CP) src/Info.plist $@/Contents
	mkdir $@/Contents/MacOS
	lipo -create $@/dicomsel.x86 $@/dicomsel.ppc \
	     -output $@/Contents/MacOS/dicomsel
	strip -ur $@/Contents/MacOS/dicomsel
	echo -n "APPL????" > $@/Contents/PkgInfo
	mkdir $@/Contents/Resources
	$(CP) src/images/icon.icns $@/Contents/Resources
	rm -f $@/dicomsel.x86 $@/dicomsel.ppc
	$(MAKE) distclean

clean:
	@echo "Deleting distribution directory..."
	$(RM) -r $(BINDIR) $(WXMSW) $(WXMAC) $(BUILDDIR)

head: head/info.txt
	@echo "Regenerating files head..."
	/bin/bash tools/gencom.sh -i head/info.txt -l head/license.txt \
		  configure.ac Makefile.am extra.mk rc.am doc/Makefile.am \
		  libdicom/Makefile.am libdicom/src/Makefile.am \
		  Makefile.msc libdicom/src/Makefile.msc msvc/flags.mak \
		  src include rfsample

head/info.txt: head/info.txt.in configure.ac
	$(MAKE) -f extra.mk configure
	./configure
	$(CP) $@ $@.save
	$(MAKE) distclean
	$(MV) $@.save $@

wxsrc: $(WXSRC)
	@if test -z "$<" -o ! -f "$<"; then \
	    echo "********************************************************"; \
	    echo "* You must specify the wxWidgets sources archive"; \
	    echo "* location through the WXSRC variable, like this:"; \
	    echo "*     $(MAKE) WXSRC=/path/to/sources.tar.bz2 <target>"; \
	    echo "* Also make sure you have MinGW installed if you"; \
	    echo "* cross-compile wxWidgets for Windows under Linux."; \
	    echo "********************************************************"; \
	    exit 1; \
	fi

$(BUILDDIR): $(WXSRC)
	$(MAKE) $(EMF) wxsrc
	rm -fr $@
	mkdir -p $@
	tar -xjf $(WXSRC) -C $@

$(WXMSWMINGW): $(BUILDDIR)
	@echo "Compiling wxWidgets for Windows..."
	rm -fr $@
	mkdir -p $@
	CWD=`pwd`; cd $(BUILDDIR)/wx* && \
	./configure --prefix="$$CWD/$@" \
		    --host=mingw32 --target=mingw32 \
		    CFLAGS="$(X86FLAGS) $(CFLAGS)" \
		    CXXFLAGS="$(X86FLAGS) $(CXXFLAGS)" \
		    $(WXCFG_FLAGS) $(WXCFG_MSW)
	$(MAKE) -C $(BUILDDIR)/wx* all
	$(MAKE) -C $(BUILDDIR)/wx* install
	$(MAKE) -C $(BUILDDIR)/wx* distclean
	rm -f $(BUILDDIR)/wx*/configarg.cache

$(WXMSWMSVC): $(WXSRC)
	$(MAKE) $(EMF) wxsrc
	rm -rf $@ $@.org
	mkdir -p $@.org
	tar -xjf $(WXSRC) -C $@.org
	mv -f $@.org/wx* $@
	rmdir $@.org
	/bin/sh tools/wxmsvc.sh $@
	touch $@

$(WXMACX86): $(BUILDDIR)
	@echo "Compiling wxWidgets for Mac OS X (x86)..."
	rm -fr $@
	mkdir -p $@
	CWD=`pwd`; cd $(BUILDDIR)/wx* && \
	./configure --prefix="$$CWD/$@" CC="$(MACX86CC)" CXX="$(MACX86CXX)" \
		    CFLAGS="$(MACX86CFLAGS) $(CFLAGS)" \
		    CXXFLAGS="$(MACX86CFLAGS) $(CXXFLAGS)" \
		    CPPFLAGS="$(MACX86CPPFLAGS)" \
		    LDFLAGS="$(MACX86LDFLAGS)" \
		    $(WXCFG_FLAGS) $(WXCFG_MAC)
	$(MAKE) -C $(BUILDDIR)/wx* all
	$(MAKE) -C $(BUILDDIR)/wx* install
	$(MAKE) -C $(BUILDDIR)/wx* distclean
	rm -f $(BUILDDIR)/wx*/configarg.cache
	sed -e "s/-Wl,-syslibroot,[^ ]*//" -i.bak $@/lib/wx/config/*

$(WXMACPPC): $(BUILDDIR)
	@echo "Compiling wxWidgets for Mac OS X (PowerPC)..."
	rm -fr $@
	mkdir -p $@
	CWD=`pwd`; cd $(BUILDDIR)/wx* && \
	./configure --prefix="$$CWD/$@" CC="$(MACPPCCC)" CXX="$(MACPPCCXX)" \
		    CFLAGS="$(MACPPCCFLAGS) $(CFLAGS)" \
		    CXXFLAGS="$(MACPPCCFLAGS) $(CXXFLAGS)" \
		    CPPFLAGS="$(MACPPCCPPFLAGS)" \
		    LDFLAGS="$(MACPPCLDFLAGS)" \
		    $(WXCFG_FLAGS) $(WXCFG_MAC) \
		    --disable-precomp-headers
	$(MAKE) -C $(BUILDDIR)/wx* all
	$(MAKE) -C $(BUILDDIR)/wx* install
	$(MAKE) -C $(BUILDDIR)/wx* distclean
	rm -f $(BUILDDIR)/wx*/configarg.cache
	sed -e "s/-Wl,-syslibroot,[^ ]*//" -i.bak $@/lib/wx/config/*

$(WXMAC):
	$(MAKE) $(EMF) $(WXMACX86)
	$(MAKE) $(EMF) $(WXMACPPC)
	touch $@

# End of File
