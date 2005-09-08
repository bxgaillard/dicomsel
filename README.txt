DicomSel: DICOM Image Selector

Copyright (C) 2005 IRCAD
Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz


DESCRIPTION

DicomSel is an application that permits you to view a series of DICOM images
and to select one of them in order to export its tags to a text file.  This
application is intended to be used alongside Radiofrequency to let you put the
needle in the right place inside the liver.

DicomSel can run on virtually any planform supported by the wxWidgets library
on which it's based.


REQUIREMENTS

On non-Windows platforms (actually UNIX ones), the wxWidgets >= 2.6.0 library
must be installed to compile DicomSel.  On Windows, this is not required,
since a precompiled build is already embeded within this package.


COMPILATION

DicomSel has been designed to be compatible with most compilers, although it
has been exclusively developed with GCC.  It is written in plain standard C++,
without using proprietary extensions, so it's expected to build against
virtually all compilers.

On the other hand, the makefile system has been written only for the GNU
version of the make utility, which is required to build DicomSel without any
modification to the makefiles.  GNU make own extensions are used very often.

So, to build DicomSel easily, just use GNU tools.  Under UNIX environments,
they are very spread, so you certainly have them already installed, especially
if you use a GNU operating system (GNU/Linux or GNU/Hurd).  Under Windows,
install MinGW (included in Dev-Cpp) or Cygwin.  Once it's done, just run
"make" in the top directory (the one including this file).  If you have MinGW
without MSYS installed, use "mingw32-make" instead.  The resulting executable
program is located in the "src" directory: "src/dicomsel" ("src\dicomsel.exe"
under Windows).

DicomSel should be compilable under Mac OS X.  Just launch "make" as under
other Unices.  To compile universal binaries, add "-arch ppc -arch i386" to
your CXXFLAGS/LDFLAGS.  For exemple, use the following command line:
    CXXFLAGS='-arch ppc -arch i386 -O2 -fomit-frame-pointer -pipe' \
    LDFLAGS='-arch ppc -arch i386' make
Note that wxWidgets must be compiled and wx-config launchable with your
current $PATH.  Report to wxWidget documentation for further informations.


CROSS-COMPILATION

As of version 1.1.0, DicomSel can be directly compiled for Windows in a UNIX
environment (however, this has only been tested on Linux).  For this to work,
you need the MinGW cross-compiler on your host platform.  Search for a package
named "mingw" or "xmingw" for your distribution (under Gentoo Linux, try
"emerge xmingw-runtime && emerge -1 xmingw-gcc && env-update" as root; do not
forget to ". /etc/profile" to let xmingw be in your $PATH).

Once this is done, test if the cross-compiler is available :
    i386-mingw32msvc-gcc --version
If the above command introduced gcc to you, you're able to cross-compile!

Then just go to the root of the DicomSel sources and type "make windows".


FILE HIERARCHY DESCRIPTION

Files are organized as follows:
    3rdparty:  third party libraries (currently OpenCow and wxWidgets).
    dist:      final application executables and dependent libraries.
    doc:       some documentation.
    head:      informations included at the top of each source file.
    libdicom:  DICOM reader library (libdicom).
    mkfiles:   makefile system.
    rfsample:  IPC sample program for Radiofrequency integration.
    src:       DicomSel application sources.
    tools:     auxiliary tools (not part of the project).


(CROSS-)BUILDING WXWIDGETS

The "tools" directory hosts a script, "xbuildwx.sh", able to build wxWidgets
very easily.  Use the following syntax:
    /bin/bash tools/xbuildwx <path to the wxWidgets sources archive>
			     -d <destination directory>

You can also type "make wxmsw" in the DicomSel sources root, this will build
wxWidgets and copy the necessary files into "3rdparty/wxwidgets" automagically
for you.


MAKING UP THE DISTRIBUTION DIRECTORY

By using "make dist", provided that you have the necessary cross-compilation
toolchain, you'll be able to make up the "dist" directory, which will contain
ready-to-use binaries for Linux x86 and Windows.


LAST WORDS

We hope you'll enjoy our project!
