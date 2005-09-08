# ----------------------------------------------------------------------------
#
# DicomSel: DICOM Image Selector
#
# Copyright (C) 2005 IRCAD
# Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
#
# ----------------------------------------------------------------------------
#
# << LICENCE >>
#
# ----------------------------------------------------------------------------


SUBDIRS = libdicom$(DIRSEP)src

include mkfiles/rules.mk

src: libdicom$(DIRSEP)src
rfsample: src

.PHONY: allos dist distclean head wxmsw

# Flags for generic x86 builds
allos: CFLAGS = -march=i386 -mtune=i686 -O2 -fweb -frename-registers \
		-fomit-frame-pointer -pipe
allos: clean
	@echo "===================== Building for Linux ====================="
	$(strip $(MAKE) CFLAGS="$(CFLAGS)" WXFLAGS="--unicode=no --debug=no" \
		unix)
	$(strip $(MAKE) cleantmp)
	@echo "==================== Building for Windows ===================="
	$(strip $(MAKE) CFLAGS="$(CFLAGS)" windows)
	$(strip $(MAKE) cleantmp)

dist: allos
	@echo "Populating distribution directory..."
	$(strip $(MV) src/dicomsel rfsample/rf dist/linux)
	$(strip $(MV) src/dicomsel.exe rfsample/rf.exe dist/win32)

macuniv: CXXFLAGS += -fweb -frename-registers -arch ppc -arch i386
macuniv: LDFLAGS  += -arch ppc -arch i386
macuniv:
	$(strip $(MAKE) CXXFLAGS="-arch ppc -arch i386 -O2 -fweb \
	    -frename-registers -fomit-frame-pointer -pipe" \
	    LDFLAGS="-arch ppc -arch i386")

macdist: all
	@echo "Creating Mac OS X .app directories under dist/mac..."
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

distclean:
	@echo "Cleaning distribution directory..."
    ifneq ($(strip $(wildcard dist/linux/* dist/win32/* dist/mac/*)),)
	$(strip $(RM) -r $(wildcard dist/linux/* dist/win32/* dist/mac/*))
    endif

head:
	@echo "Regenerating files head..."
	$(strip /bin/bash tools/gencom.sh -i head/info.txt -l \
		head/licence.txt GNUmakefile mkfiles src rfsample \
		libdicom/src/GNUmakefile)

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
    WXDIR = 3rdparty/wxmsw
    WXARC := $(word $(words $(WXARC)),$(WXARC))
    wxmsw:
	@echo "Compiling a light version of wxWidgets for Windows..."
	$(strip $(RM) -r $(WXDIR))
	$(strip /bin/bash tools/xbuildwx.sh -d $(WXDIR) $(WXARC) \
		$(WXCFG_FLAGS))
	$(strip $(RM) $(WXDIR)/lib/libwx_base_net-*.a \
		      $(WXDIR)/lib/libwx_base_xml-*.a \
		      $(WXDIR)/lib/libwx_msw_adv-*.a \
		      $(WXDIR)/lib/libwx_msw_media-*.a)
else
    wxmsw:
	@echo "Sorry, this only works automatically on Gentoo Linux."
	@echo "For other distribution, you have to specify the sources"
	@echo "archive location like this:"
	@echo "    $(MAKE) WXSRC=/path/to/sources.tar.bz2 wxmsw"
	@echo "Also, make sure you have MinGW to cross-compile wxWidgets."
	@exit 1
endif

# End of File
