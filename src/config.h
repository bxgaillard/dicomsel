/*
 * ---------------------------------------------------------------------------
 *
 * DicomSel: DICOM Image Selector
 *
 * Copyright (C) 2005 IRCAD
 * Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
 *
 * ---------------------------------------------------------------------------
 *
 * << LICENCE >>
 *
 * ---------------------------------------------------------------------------
 */


#ifndef DICOMSEL_CONFIG_H
#define DICOMSEL_CONFIG_H

/*
 * IPC Settings
 */
#define IPC_HOSTNAME   "localhost"
#define IPC_SERVICE    "Radiofrequency"
//#define IPC_UNIXSOCKET "/tmp/dicomsel" // Better but seems buggy...
#define IPC_UNIXSOCKET "17523" // 'Ds' (0x4473)
#define IPC_TOPIC      "SendPosition"
#define IPC_ITEM       "Position"

#endif // !DICOMSEL_CONFIG_H

/* End of File */
