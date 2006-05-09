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

CONFIGURE = ./configure

CFLAGS = -march=i386 -mtune=i686 -O2 -fomit-frame-pointer -pipe
CXXFLAGS = $(CFLAGS) -fno-rtti

BINDIR = bin
BUILDDIR = build
WXDIR = wxmsw
PWD ?= $(shell pwd)
PWD := $(PWD)

.PHONY: default all autogen autoclean clean head
.PHONY: dist bindist linux win32 win32-mingw win32-msc mac

default: dist bindist autoclean
all: default autoclean

configure: autogen

autogen:
	test -d autotools || mkdir autotools
	aclocal
	autoconf
	autoheader
	automake -a -c

autoclean:
	test ! -f Makefile || make distclean
	rm -rf autom4te.cache config.h.in aclocal.m4 configure autotools \
	    `find -name Makefile.in | grep -v ^\\./3rdparty`

dist: configure
	./configure
	$(MAKE) dist-bzip2
	$(MAKE) distclean

bin: linux win32

bin.zip: bin
	find $< | LC_ALL=C sort | zip -9 -@ $@

bindist: bin.zip
	rm -rf bin

linux: configure
	test -d $(BINDIR) || mkdir $(BINDIR)
	@echo "===================== Building for Linux ====================="
	$(CONFIGURE) CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="-s"
	$(MAKE)
	test -d $(BINDIR)/linux || mkdir -p $(BINDIR)/linux
	$(CP) src/dicomsel $(BINDIR)/linux
	$(MAKE) distclean

win32:
	@echo "==================== Building for Windows ===================="
	$(MAKE) -f extra.mk win32-msc || $(MAKE) -f extra.mk win32-mingw

win32-mingw: configure wxmsw
	$(CONFIGURE) --host=i386-mingw32msvc --target=i386-mingw32msvc \
		     --with-wx-prefix=$(PWD)/wxmsw \
		     CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" \
		     CPPFLAGS="-DLITTLE_ENDIAN=1234 -DBIG_ENDIAN=4321 \
			       -DBYTE_ORDER=1234" LDFLAGS="-s"
	$(MAKE)
	test -d $(BINDIR)/win32 || mkdir -p $(BINDIR)/win32
	$(CP) src/dicomsel.exe $(BINDIR)/win32
	$(MAKE) distclean

win32-msc: msvc/config.h msvc/flags.mak
	env -u MAKEFLAGS wine nmake /NOLOGO /f Makefile.msc
	test -d $(BINDIR)/win32 || mkdir -p $(BINDIR)/win32
	$(CP) src/dicomsel.exe $(BINDIR)/win32
	env -u MAKEFLAGS wine nmake /NOLOGO /f Makefile.msc clean

msvc/config.h msvc/flags.mak: msvc/config.h.in msvc/flags.mak.in configure.ac
	$(MAKE) -f extra.mk configure
	$(CONFIGURE)
	$(CP) msvc/config.h msvc/config.h.save
	$(CP) msvc/flags.mak msvc/flags.mak.save
	$(MAKE) distclean
	$(MV) msvc/config.h.save msvc/config.h
	$(MV) msvc/flags.mak.save msvc/flags.mak

UNIV = -arch ppc -arch i386
mac: configure
	@echo "=================== Building for Mac OS X ===================="
	$(CONFIGURE) CFLAGS="$(CFLAGS) $(UNIV)" \
		     CXXFLAGS="$(CXXFLAGS) $(UNIV)"
		     LDFLAGS="$(UNIV) -s"
	$(MAKE)
	@echo "Creating Mac OS X .app directories under $(BINDIR)/mac..."
	test -d $(BINDIR)/mac || mkdir -p $(BINDIR)/mac
	$(RM) -r $(BINDIR)/mac/DicomSel.app
	mkdir $(BINDIR)/mac/DicomSel.app
	mkdir $(BINDIR)/mac/DicomSel.app/Contents
	$(CP) src/Info.plist $(BINDIR)/mac/DicomSel.app/Contents
	mkdir $(BINDIR)/mac/DicomSel.app/Contents/MacOS
	$(CP) src/dicomsel $(BINDIR)/mac/DicomSel.app/Contents/MacOS
	echo -n "APPL????" > $(BINDIR)/mac/DicomSel.app/Contents/PkgInfo
	mkdir $(BINDIR)/mac/DicomSel.app/Contents/Resources
	$(CP) src/images/icon.icns \
	      $(BINDIR)/mac/DicomSel.app/Contents/Resources
#	$(RM) -r "$(BINDIR)/mac/RadioFrequency Sample.app"
#	mkdir "$(BINDIR)/mac/Radiofrequency Sample.app"
#	mkdir "$(BINDIR)/mac/Radiofrequency Sample.app/Contents"
#	$(CP) rfsample/Info.plist \
#	      "$(BINDIR)/mac/Radiofrequency Sample.app/Contents"
#	mkdir "$(BINDIR)/mac/Radiofrequency Sample.app/Contents/MacOS"
#	$(CP) rfsample/rf \
#	      "$(BINDIR)/mac/Radiofrequency Sample.app/Contents/MacOS"
#	echo -n "APPL????" \
#	     > "$(BINDIR)/mac/Radiofrequency Sample.app/Contents/PkgInfo"
#	$(MAKE) distclean

clean:
	@echo "Deleting distribution directory..."
	$(RM) -r $(BINDIR) $(WXDIR) $(BUILDDIR)

head: head/info.txt
	@echo "Regenerating files head..."
	/bin/bash tools/gencom.sh -i head/info.txt -l head/license.txt \
		  configure.ac Makefile.am extra.mk rc.am doc/Makefile.am \
		  libdicom/Makefile.am libdicom/src/Makefile.am \
		  Makefile.msc libdicom/src/Makefile.msc msvc/flags.mak \
		  src include rfsample

head/info.txt: head/info.txt.in configure.ac
	$(MAKE) -f extra.mk configure
	$(CONFIGURE)
	$(CP) $@ $@.save
	$(MAKE) distclean
	$(MV) $@.save $@

# Works with Gentoo Linux only
ifeq ($(WXSRC),)
    WXSRC = /usr/portage/distfiles/wxWidgets-*.tar.bz2
endif

WXARC := $(sort $(wildcard $(WXSRC)))
WXCFG_FLAGS = --disable-shared --enable-no_rtti --enable-no_exceptions \
	      --enable-no_deps --disable-compat24 --disable-rpath \
	      --disable-sockets --disable-unicode --disable-threads \
	      --disable-html --disable-xrc --disable-debugreport \
	      --disable-postscript --disable-calendar --disable-datepick \
	      --disable-grid --disable-help --disable-joystick \
	      --disable-sash --disable-sound --disable-splash \
	      --disable-tipdlg --disable-wizarddlg --disable-gif \
	      --disable-pnm --disable-pcx --without-libpng --without-libjpeg \
	      --without-libtiff --without-regex --without-zlib --without-expat

ifneq ($(WXARC),)
    WXARC := $(word $(words $(WXARC)),$(WXARC))
    wxmsw:
	@echo "Compiling a light version of wxWidgets for Windows..."
	$(RM) -r $(WXDIR) $(BUILDDIR)
	mkdir $(BUILDDIR)
	tar -xjf $(WXARC) -C $(BUILDDIR)
	cd $(BUILDDIR)/wx* && ./configure --prefix=$(PWD)/$(WXDIR) \
	    --host=i386-mingw32msvc --target=i386-mingw32msvc \
	    CFLAGS="$(CFLAGS)" CXXFLAGS="$(CFLAGS)" --with-msw $(WXCFG_FLAGS)
	$(MAKE) -C $(BUILDDIR)/wx* install
	$(RM) -r $(BUILDDIR)

#	@echo "Compiling a light version of wxWidgets for Windows..."
#	$(RM) -r $(WXDIR)
#	/bin/bash tools/xbuildwx.sh -d $(WXDIR) $(WXARC) $(WXCFG_FLAGS)
#	$(RM) $(WXDIR)/lib/libwx_base_net-*.a \
#	      $(WXDIR)/lib/libwx_base_xml-*.a \
#	      $(WXDIR)/lib/libwx_msw_adv-*.a \
#	      $(WXDIR)/lib/libwx_msw_media-*.a
else
    wxmsw:
	@echo "Sorry, this only works automatically on Gentoo Linux."
	@echo "For other distributions, you have to specify the sources"
	@echo "archive location like this:"
	@echo "    $(MAKE) WXSRC=/path/to/sources.tar.bz2 wxmsw"
	@echo "Also, make sure you have MinGW to cross-compile wxWidgets."
	@exit 1
endif

# End of File
