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


// wxWidgets
#include <wx/wxprec.h>
#ifdef __BORLANDC__
# pragma hdrstop
#endif
//#ifndef WX_PRECOMP
# include <wx/panel.h>
# include <wx/window.h>
# include <wx/gdicmn.h>
# include <wx/string.h>
# include <wx/dcbuffer.h>
# include <wx/brush.h>
# include <wx/settings.h>
# include <wx/image.h>
# include <wx/bitmap.h>
# include <wx/dcmemory.h>
//#endif

// Current module
#include <dicomsel/BitmapPanel.h>


namespace dicomsel
{

BEGIN_EVENT_TABLE( BitmapPanel, wxPanel )
    EVT_PAINT( BitmapPanel::OnPaint )
    EVT_SIZE ( BitmapPanel::OnSize  )
END_EVENT_TABLE()

BitmapPanel::BitmapPanel( wxWindow* const parent, const wxWindowID id,
			  bool zoom, const wxPoint& pos, const wxSize& size,
			  long style, const wxString& name )
:   wxPanel( parent, id, pos, size, style | wxNO_FULL_REPAINT_ON_RESIZE,
	     name ),
    m_zoom( zoom )
{}

BitmapPanel::~BitmapPanel( void )
{}

void BitmapPanel::SetImage( const wxImage& image )
{
    if( image.Ok() )
    {
	m_image = image;
	Rescale( true );
    }
    else Clear();
}

void BitmapPanel::SetImage( const wxBitmap& bitmap )
{
    if( bitmap.Ok() ) SetImage( bitmap.ConvertToImage() );
    else Clear();
}

void BitmapPanel::Clear( void )
{
    if( m_image.Ok() )
    {
	m_memDC.SelectObject( wxNullBitmap );
	m_image.Destroy();

	// Redraw window
	Refresh();
    }
}

void BitmapPanel::Rescale( bool force )
{
    if( !m_image.Ok() ) return;
    const wxSize size = GetClientSize();
    wxSize imageSize;

    if( m_zoom ||
	size.GetWidth()  < m_image.GetWidth() ||
	size.GetHeight() < m_image.GetHeight() )
    {
	// Window is smaller than image

	// Find correct image dimension
	if( size.GetWidth() * m_image.GetHeight()
	    >= size.GetHeight() * m_image.GetWidth() )
	{
	    // Wide window, high bitmap
	    imageSize.Set( size.GetHeight() * m_image.GetWidth()
			   / m_image.GetHeight(), size.GetHeight() );
	}
	else
	{
	    // High window, wide bitmap
	    imageSize.Set( size.GetWidth(), size.GetWidth()
			   * m_image.GetHeight() / m_image.GetWidth() );
	}
    }
    else
    {
	// Image is smaller than window

	// Keep image dimension
	imageSize.Set( m_image.GetWidth(), m_image.GetHeight() );
    }

    // Center image
    if( imageSize.GetWidth() >= size.GetWidth() ) m_pos.x = 0;
    else m_pos.x = (size.GetWidth() - imageSize.GetWidth()) / 2;
    if( imageSize.GetHeight() >= size.GetHeight() ) m_pos.y = 0;
    else m_pos.y = (size.GetHeight() - imageSize.GetHeight()) / 2;

    // Only rescale if size is different
    const wxSize dcSize = m_memDC.GetSize();
    if( force || !dcSize.IsFullySpecified() ||
	imageSize.GetWidth() != dcSize.GetWidth() ||
	imageSize.GetHeight() != dcSize.GetHeight() )
    {
	// Create a scaled version
	wxImage image =
	    m_image.Scale( imageSize.GetWidth(), imageSize.GetHeight() );

	// Pre-draw in temporary drawing context
	m_memDC.SelectObject( wxBitmap( image.GetWidth(),
					image.GetHeight() ) );
	m_memDC.DrawBitmap( wxBitmap( image ), 0, 0 );
    }

    // Redraw image
    Refresh(false);
}

void BitmapPanel::OnPaint( wxPaintEvent& event )
{
    if( !m_image.Ok() )
    {
	event.Skip();
	return;
    }

    const wxSize size = m_memDC.GetSize(), winSize = GetClientSize();
    wxPaintDC dc( this );

    dc.SetPen( wxPen( *wxBLACK, 0, wxTRANSPARENT ) );
    dc.SetBrush( wxBrush( wxSystemSettings::GetColour(
	wxSYS_COLOUR_WINDOWFRAME ) ) );

    if( size.GetHeight() < winSize.GetHeight() )
    {
	if( m_pos.y > 0 )
	{
	    dc.DrawRectangle( 0, 0, winSize.GetWidth(), m_pos.y );
	}
	dc.DrawRectangle( 0, m_pos.y + size.GetHeight(), winSize.GetWidth(),
			  winSize.GetHeight() - size.GetHeight() - m_pos.y );
    }

    if( size.GetWidth() < winSize.GetWidth() )
    {
	if( m_pos.x > 0 )
	{
	    dc.DrawRectangle( 0, m_pos.y, m_pos.x, size.GetHeight() );
	}
	dc.DrawRectangle( m_pos.x + size.GetWidth(), m_pos.y,
			  winSize.GetWidth() - size.GetWidth() - m_pos.x,
			  size.GetHeight() );
    }

    dc.Blit( m_pos.x, m_pos.y, size.GetWidth(), size.GetHeight(), &m_memDC,
	     0, 0 );
}

void BitmapPanel::OnSize( wxSizeEvent& event )
{
    event.Skip();

    Rescale();
}

} // namespace dicomsel

/* End of File */
