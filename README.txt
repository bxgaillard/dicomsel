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

To be able to run DicomSel, nothing special is required.  But if you use an
older version of Windows (95, 98 or ME), you need an additional library from
Microsoft called MSLU, available here:
    http://www.microsoft.com/globaldev/handson/dev/mslu_announce.mspx
Due to its licence terms, we cannot include it here; we apologize for the
incovenience.

An alternative to this is to use the library included in the "3rdparty"
directory.  It's a free replacement of the proprietary Microsoft library.  The
application executable and this library shall be located in the same directory
to work together.

Note that the preceding is useless on Windows NT/2000/XP.

On non-Windows platforms (actually UNIX ones), the wxWidgets >= 2.6.0 library
must be installed to compile DicomSel.  On Windows, this is not required,
since a precompiled one is alredy embeded within this package.


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


FILE HIERARCHY DESCRIPTION

Files are organized as follows:
    3rdparty:  third party libraries.
    dist:      final application executables and dependent libraries.
    doc:       some documentation.
    head:      informations included at the top of each source file.
    libdicom:  DICOM reader library (libdicom).
    mkfiles:   makefile system.
    src:       DicomSel application sources.
    tools:     auxiliary tools (not part of the project).
    wxwidgets: precompiled wxWidgets library for Win32 platform.


LAST WORDS

We hope you'll enjoy our project!
