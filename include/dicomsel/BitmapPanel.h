/*
 * ---------------------------------------------------------------------------
 *
 * DicomSel: DICOM Image Selector
 *
 * Copyright (C) 2005, 2006 IRCAD
 * Authors: Benjamin Gaillard, Marc-Aurèle Mörk, Guillaume Spitz
 *
 * ---------------------------------------------------------------------------
 *
 * << LICENSE >>
 *
 * ---------------------------------------------------------------------------
 */


#ifndef DICOMSEL_BITMAPPANEL_H
#define DICOMSEL_BITMAPPANEL_H

// wxWidgets
#ifndef WX_PRECOMP
# include <wx/panel.h>
# include <wx/window.h>
# include <wx/gdicmn.h>
# include <wx/string.h>
# include <wx/image.h>
# include <wx/dcmemory.h>
#endif // !WX_PRECOMP

class wxBitmap;
class wxPaintEvent;
class wxSizeEvent;


namespace dicomsel
{

class BitmapPanel : public wxPanel
{
public:
    BitmapPanel( wxWindow* const parent, const wxWindowID id = -1,
		 bool zoom = true, const wxPoint& pos = wxDefaultPosition,
		 const wxSize& size = wxDefaultSize,
		 long style = wxSUNKEN_BORDER,
		 const wxString& name = wxT( "BitmapPanel" ) );
    virtual ~BitmapPanel( void );

    void SetImage( const wxImage& image );
    void SetImage( const wxBitmap& bitmap );
    void Clear( void );

private:
    bool       m_zoom;
    wxImage    m_image;
    wxMemoryDC m_memDC;
    wxPoint    m_pos;

    void Rescale( bool force = false );

    void OnPaint( wxPaintEvent& event );
    void OnSize ( wxSizeEvent&  event );

    DECLARE_EVENT_TABLE()
};

} // namespace dicomsel

#endif // DICOMSEL_BITMAPPANEL_H

/* End of File */
