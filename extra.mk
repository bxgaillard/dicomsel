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
#WXFLAGS="--unicode=no --debug=no" unix

BUILDDIR = build
WXDIR = wxmsw
PWD ?= $(shell pwd)
PWD := $(PWD)

.PHONY: linux win32 mac distclean head

# Flags for generic x86 builds
dist: linux win32

linux:
	test -d dist || mkdir dist
	@echo "===================== Building for Linux ====================="
	$(CONFIGURE) CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="-s"
	$(MAKE)
	test -d dist/linux || mkdir -p dist/linux
	$(CP) src/dicomsel rfsample/rf dist/linux
	$(MAKE) distclean

win32: wxmsw
	@echo "==================== Building for Windows ===================="
	$(CONFIGURE) --host=i386-mingw32msvc --target=i386-mingw32msvc \
		     --with-wx-prefix=$(PWD)/wxmsw \
		     CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" \
		     CPPFLAGS="-DLITTLE_ENDIAN=1234 -DBIG_ENDIAN=4321 \
			       -DBYTE_ORDER=1234" LDFLAGS="-s"
	$(MAKE)
	test -d dist/win32 || mkdir -p dist/win32
	$(CP) src/dicomsel.exe rfsample/rf.exe dist/win32
	$(MAKE) distclean

UNIV = -arch ppc -arch i386
mac: all
	@echo "=================== Building for Mac OS X ===================="
	$(CONFIGURE) CFLAGS="$(CFLAGS) $(UNIV)" \
		     CXXFLAGS="$(CXXFLAGS) $(UNIV)"
		     LDFLAGS="$(UNIV) -s"
	$(MAKE)
	@echo "Creating Mac OS X .app directories under dist/mac..."
	test -d dist/mac || mkdir -p dist/mac
	$(RM) -r dist/mac/DicomSel.app
	mkdir dist/mac/DicomSel.app
	mkdir dist/mac/DicomSel.app/Contents
	$(CP) src/Info.plist dist/mac/DicomSel.app/Contents
	mkdir dist/mac/DicomSel.app/Contents/MacOS
	$(CP) src/dicomsel dist/mac/DicomSel.app/Contents/MacOS
	echo -n "APPL????" > dist/mac/DicomSel.app/Contents/PkgInfo
	mkdir dist/mac/DicomSel.app/Contents/Resources
	$(CP) src/images/icon.icns dist/mac/DicomSel.app/Contents/Resources
	$(RM) -r "dist/mac/RadioFrequency Sample.app"
	mkdir "dist/mac/Radiofrequency Sample.app"
	mkdir "dist/mac/Radiofrequency Sample.app/Contents"
	$(CP) rfsample/Info.plist \
	      "dist/mac/Radiofrequency Sample.app/Contents"
	mkdir "dist/mac/Radiofrequency Sample.app/Contents/MacOS"
	$(CP) rfsample/rf "dist/mac/Radiofrequency Sample.app/Contents/MacOS"
	echo -n "APPL????" \
	     > "dist/mac/Radiofrequency Sample.app/Contents/PkgInfo"
	$(MAKE) distclean

distclean:
	@echo "Deleting distribution directory..."
	$(RM) -r dist $(WXDIR) $(BUILDDIR)

head:
	@echo "Regenerating files head..."
	/bin/bash tools/gencom.sh -i head/info.txt -l head/license.txt \
		  configure.ac Makefile.am extra.mk doc/Makefile.am \
		  libdicom/Makefile.am libdicom/src/Makefile.am \
		  src include rfsample

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
