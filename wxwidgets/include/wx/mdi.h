/////////////////////////////////////////////////////////////////////////////
// Name:        mdi.h
// Purpose:     wxMDI base header
// Author:      wxWidgets Team
// Modified by:
// Created:
// Copyright:   (c) wxWidgets Team
// RCS-ID:      $Id: mdi.h,v 1.17 2005/03/16 16:18:19 ABX Exp $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MDI_H_BASE_
#define _WX_MDI_H_BASE_

#include "wx/defs.h"

#if wxUSE_MDI

#if defined(__WXUNIVERSAL__)
    #include "wx/generic/mdig.h"
#elif defined(__WXMSW__)
    #include "wx/msw/mdi.h"
#elif defined(__WXMOTIF__)
    #include "wx/motif/mdi.h"
#elif defined(__WXGTK__)
    #include "wx/gtk/mdi.h"
#elif defined(__WXMAC__)
    #include "wx/mac/mdi.h"
#elif defined(__WXCOCOA__)
    #include "wx/cocoa/mdi.h"
#elif defined(__WXPM__)
    #include "wx/generic/mdig.h"
#endif

#endif // wxUSE_MDI

#endif
    // _WX_MDI_H_BASE_
