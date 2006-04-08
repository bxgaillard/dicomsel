# ----------------------------------------------------------------------------
#
# DicomSel: DICOM Image Selector
#
# Copyright (C) 2005, 2006 IRCAD
# Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
#
# ----------------------------------------------------------------------------
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc., 59
# Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# ----------------------------------------------------------------------------


AM_CPPFLAGS = -I$(srcdir)/../include -I$(srcdir)/../include/dicom

noinst_LIBRARIES = libdicom.a
libdicom_a_SOURCES = \
    DicomAbstractFactory.cpp \
    DicomDictionary.cpp \
    ExceptionDicom.cpp \
    io/DicomFile.cpp \
    io/SimpleDirectory.cpp \
    io/SimpleFile.cpp \
    io/TypedFile.cpp \
    lib/DicomLib.cpp \
    tag/AcquisitionDate.cpp \
    tag/AcquisitionNumber.cpp \
    tag/AcquisitionTime.cpp \
    tag/AcquisitionZone.cpp \
    tag/Birthday.cpp \
    tag/BitsAllocated.cpp \
    tag/Columns.cpp \
    tag/DicomTag.cpp \
    tag/Hospital.cpp \
    tag/ImagePosition.cpp \
    tag/ImageType.cpp \
    tag/Modality.cpp \
    tag/Name.cpp \
    tag/PatientID.cpp \
    tag/PixelData.cpp \
    tag/PixelRepresentation.cpp \
    tag/PixelSpacing.cpp \
    tag/Rows.cpp \
    tag/SeriesUID.cpp \
    tag/Sexe.cpp \
    tag/SliceThickness.cpp \
    tag/StudyUID.cpp \
    tag/TagWithStringValue.cpp \
    tag/TransfertSyntax.cpp \
    tag/WindowWidth.cpp \
    visitor/AbstractDicomFile.cpp \
    visitor/CreateImage.cpp \
    visitor/CreateInrimage.cpp \
    visitor/CreateList.cpp \
    visitor/Dicom.cpp

EXTRA_DIST = \
    Makefile.msc

# End of File
