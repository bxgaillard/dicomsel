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

.PHONY: head
head:
	$(strip /bin/bash ./tools/gencom.sh -i head/info.txt -l \
		head/licence.txt GNUmakefile mkfiles src \
		libdicom/src/GNUmakefile)

# End of File
