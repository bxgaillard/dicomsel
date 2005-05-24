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


// wxWidgets
#include <wx/wxprec.h>
#ifdef __BORLANDC__
# pragma hdrstop
#endif
#ifndef WX_PRECOMP
# include <wx/string.h>
# include <wx/strconv.h>
# include <wx/gdicmn.h>
# include <wx/frame.h>
# include <wx/event.h>
# include <wx/icon.h>
# include <wx/menu.h>
# include <wx/menuitem.h>
# include <wx/statusbr.h>
# include <wx/splitter.h>
# include <wx/panel.h>
# include <wx/sizer.h>
# include <wx/statbox.h>
# include <wx/config.h>
# include <wx/stattext.h>
# include <wx/filename.h>
# include <wx/dirdlg.h>
# include <wx/filedlg.h>
# include <wx/wfstream.h>
# include <wx/buffer.h>
# include <wx/msgdlg.h>
# include <wx/image.h>
#endif

// Standard C library
#include <cstring>

// Current module
#include "DicomTree.h"
#include "BitmapPanel.h"
#include "TagSet.h"
#include "TagDialog.h"
#include "DicomFile.h"
#include "version.h"
#include "MainFrame.h"


namespace dicomsel
{

// Line break is platform-specific
#if defined( __WXMSW__ ) || defined( __WXOS2__ )
# define LINE_BREAK "\r\n"
#elif defined( __WXMAC__ )
# define LINE_BREAK '\r'
#else
# define LINE_BREAK '\n'
#endif

/*
 * Menu and widgets IDs
 */
enum
{
    // Standard menu IDs
    MENU_OPENDIR  = wxID_OPEN,
    MENU_CLOSEDIR = wxID_CLOSE,
    MENU_QUIT     = wxID_EXIT,
    MENU_ABOUT    = wxID_ABOUT,

    // Custom menu IDs
    MENU_EXPORT = wxID_HIGHEST + 1,
    MENU_DISPLAYED_TAGS,
    MENU_EXPORTED_TAGS,

    // Custom widgets IDs
    WID_DICOMTREE,
    WID_BITMAP,
    WID_SPLITTER,
    WID_PANEL,
    WID_IMAGEBOX,
    WID_INFOBOX
};

/*
 * Event table
 */
BEGIN_EVENT_TABLE( MainFrame, wxFrame )
    // Frame events
    EVT_CLOSE( MainFrame::OnClose )

    // Menu events
    EVT_MENU( MENU_OPENDIR,        MainFrame::OnMenuOpenDir       )
    EVT_MENU( MENU_CLOSEDIR,       MainFrame::OnMenuCloseDir      )
    EVT_MENU( MENU_EXPORT,         MainFrame::OnMenuExport        )
    EVT_MENU( MENU_QUIT,           MainFrame::OnMenuQuit          )
    EVT_MENU( MENU_DISPLAYED_TAGS, MainFrame::OnMenuDisplayedTags )
    EVT_MENU( MENU_EXPORTED_TAGS,  MainFrame::OnMenuExportedTags  )
    EVT_MENU( MENU_ABOUT,          MainFrame::OnMenuAbout         )

    // Tree events
    EVT_TREE_SEL_CHANGED( WID_DICOMTREE, MainFrame::OnSelectionChanged )
END_EVENT_TABLE()


// Application icon (under Windows and OS/2 it's in resources)
#if !defined( __WXMSW__ ) && !defined( __WXOS2__ )
# include "images/icon.xpm"
#endif


MainFrame::MainFrame( const wxString& title, const wxPoint& pos,
		      const wxSize& size )
:   wxFrame( NULL, -1, title, pos, size, wxDEFAULT_FRAME_STYLE ),
    m_grid( NULL ),
    m_dirPath( wxGetCwd() )
{
    SetMinSize( wxSize( 300, 200 ) );
    SetIcon( wxICON( icon ) );

#if wxUSE_MENUS
    // File menu
    wxMenu* const fileMenu = new wxMenu;
    fileMenu->Append( MENU_OPENDIR, wxT( "&Open directory...\tCTRL-O" ),
		      wxT( "Scan a directory for DICOM images" ) );
    m_closeMenu = fileMenu->Append( MENU_CLOSEDIR,
				    wxT( "&Close directory\tCTRL-C" ),
				    wxT( "Close the current directory" ) );
    m_closeMenu->Enable( false );
    fileMenu->AppendSeparator();
    m_exportMenu = fileMenu->Append( MENU_EXPORT,
				     wxT( "&Export tags...\tCTRL-E" ),
				     wxT( "Export the selected tags to a "
					  "file" ) );
    m_exportMenu->Enable( false );
    fileMenu->AppendSeparator();
    fileMenu->Append( MENU_QUIT, wxT( "&Quit\tCTRL-Q" ),
		      wxT( "Exit DicomSel" ) );

    // Options menu
    wxMenu* const optionsMenu = new wxMenu;
    optionsMenu->Append( MENU_DISPLAYED_TAGS,
			 wxT( "&Displayed tags...\tCTRL-D" ),
			 wxT( "Select which DICOM tags to display alongside "
			      "the image" ) );
    optionsMenu->Append( MENU_EXPORTED_TAGS,
			 wxT( "&Exported tags...\tCTRL-T" ),
			 wxT( "Select which DICOM tags should be exported "
			      "to a file" ) );

    // Help menu
    wxMenu* const helpMenu = new wxMenu;
    helpMenu->Append( MENU_ABOUT, wxT( "&About..." ),
		      wxT( "Informations about DicomSel" ) );

    // Menu bar
    wxMenuBar* const menuBar = new wxMenuBar;
    menuBar->Append( fileMenu, wxT( "&File" ) );
    menuBar->Append( optionsMenu, wxT( "&Options" ) );
    menuBar->Append( helpMenu, wxT( "&Help" ) );
    SetMenuBar( menuBar );
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // Status bar
    CreateStatusBar( 1 );
    SetStatusText( wxT( "DICOM Image Selector" ) );
#endif // wxUSE_STATUSBAR

    // Splitter window
    wxSplitterWindow* const splitter =
	new wxSplitterWindow( this, WID_SPLITTER, wxDefaultPosition,
			      wxDefaultSize, wxSP_LIVE_UPDATE );
    splitter->SetMinimumPaneSize( 150 );

    // Tree on the left, panel on the right
    m_tree = new DicomTree( splitter, WID_DICOMTREE );
    m_panel = new wxPanel( splitter, WID_PANEL );
    splitter->SplitVertically( m_tree, m_panel, 300 );

    // Vertical sizer inside panel
    wxBoxSizer* const sizer = new wxBoxSizer( wxVERTICAL );
    m_panel->SetSizer( sizer );

    // Static box containing the image, and the image itself
    wxStaticBox* const imageBox =
	new wxStaticBox( m_panel, WID_IMAGEBOX, wxT( "Image" ) );
    wxStaticBoxSizer* const imageSizer =
	new wxStaticBoxSizer( imageBox, wxHORIZONTAL );
    sizer->Add( imageSizer, 1, wxALL ^ wxBOTTOM | wxEXPAND, 4 );
    m_bitmap = new BitmapPanel( m_panel, WID_BITMAP );
    imageSizer->Add( m_bitmap, 1, wxALL | wxEXPAND, 4 );

    // Static box containing informations (tags)
    wxStaticBox* const infoBox =
	new wxStaticBox( m_panel, WID_INFOBOX, wxT( "Informations" ) );
    m_infoSizer = new wxStaticBoxSizer( infoBox, wxVERTICAL );
    sizer->Add( m_infoSizer, 0, wxALL | wxEXPAND, 4 );

    // Load config
    LoadConfig();
}

MainFrame::~MainFrame( void )
{}

void MainFrame::LoadConfig( void )
{
    wxConfig config( wxT( "DicomSel" ) );

    m_displayedTags.Clear();
    m_exportedTags.Clear();

    for( int i = TagSet::TAG__FIRST; i < TagSet::TAG__LAST; i++ )
    {
	const TagSet::TagID tag = static_cast< TagSet::TagID >( i );
	if( config.Read( wxString( wxT( "Display/" ) ) +
			 TagSet::GetTagID( tag ),
			 tag == TagSet::TAG_IMAGE_POSITION ) )
	{
	    m_displayedTags.Add( tag );
	}
	if( config.Read( wxString( wxT( "Export/" ) ) +
			 TagSet::GetTagID( tag ),
			 tag == TagSet::TAG_IMAGE_POSITION ) )
	{
	    m_exportedTags.Add( tag );
	}

	m_labels[i].name = NULL;
	m_labels[i].value = NULL;
    }

    // Redraw tags
    RefreshDisplayedTags();
}

void MainFrame::SaveConfig( void ) const
{
    wxConfig config( wxT( "DicomSel" ) );

    for( int i = TagSet::TAG__FIRST; i < TagSet::TAG__LAST; i++ )
    {
	const TagSet::TagID tag = static_cast< TagSet::TagID >( i );
	config.Write( wxString( wxT( "Display/" ) ) + TagSet::GetTagID( tag ),
		      m_displayedTags.IsIn( tag ) );
	config.Write( wxString( wxT( "Export/" ) ) + TagSet::GetTagID( tag ),
		      m_exportedTags.IsIn( tag ) );
    }
}

void MainFrame::RefreshDisplayedTags( void )
{
    if( m_grid != NULL ) m_infoSizer->Remove( 0 );
    m_grid = new wxFlexGridSizer( m_displayedTags.GetCount(), 2, 2, 0 );
    m_grid->AddGrowableCol( 0 );
    m_grid->AddGrowableCol( 1 );
    m_infoSizer->Add( m_grid, 1, wxEXPAND );

    for( int i = TagSet::TAG__FIRST; i < TagSet::TAG__LAST; i++ )
    {
	if( (m_labels[i].name != NULL) !=
	    (m_displayedTags.IsIn( static_cast< TagSet::TagID >( i ) )) )
	{
	    if( m_labels[i].name == NULL )
	    {
		m_labels[i].name = new wxStaticText( m_panel, -1,
		    TagSet::GetTagName( static_cast< TagSet::TagID >( i ) )
						     + wxT(": " ),
						     wxDefaultPosition,
						     wxDefaultSize,
						     wxALIGN_RIGHT
						     | wxST_NO_AUTORESIZE );
		m_labels[i].value =
		    new wxStaticText( m_panel, -1, wxEmptyString );
	    }
	    else
	    {
		m_labels[i].name->Destroy();
		m_labels[i].name = NULL;
		m_labels[i].value->Destroy();
		m_labels[i].value = NULL;
	    }
	}
    }

    m_displayedTags.Rewind();
    TagSet::TagID tag;
    while( (tag = m_displayedTags.GetNext()) != TagSet::TAG__LAST )
    {
	m_grid->Add( m_labels[tag].name, 0, wxEXPAND );
	m_grid->Add( m_labels[tag].value, 0, wxRIGHT | wxEXPAND, 8 );
    }

    // Force windows resizing
    m_panel->Layout();
}

void MainFrame::RefreshLabels( void )
{
    const DicomFile& file = m_tree->GetFile();
    m_displayedTags.Rewind();
    TagSet::TagID tag;

    while( (tag = m_displayedTags.GetNext()) != TagSet::TAG__LAST )
    {
	if( m_labels[tag].value != NULL )
	{
	    m_labels[tag].value->SetLabel( &file != NULL ?
					   file.GetTagString( tag ) :
					   *wxEmptyString );
	}
    }

    // Force windows resizing
    m_panel->Layout();
}

void MainFrame::OnClose( wxCloseEvent& event )
{
    SaveConfig();
    event.Skip();
}

void MainFrame::OnMenuOpenDir( wxCommandEvent& WXUNUSED( event ) )
{
    wxDirDialog* dialog = new
	wxDirDialog( this, wxT( "Select a DICOM image directory" ),
		     m_dirPath );

    if( dialog->ShowModal() == wxID_OK )
    {
	const wxString path( dialog->GetPath() );
	dialog->Destroy();
	if( m_tree->OpenDirectory( path ) )
	{
	    m_dirPath = path;
	    m_closeMenu->Enable();
	    m_bitmap->Clear();
	    RefreshLabels();
	}
    }
    else dialog->Destroy();
}

void MainFrame::OnMenuCloseDir( wxCommandEvent& WXUNUSED( event ) )
{
    m_tree->CloseDirectory();
    m_closeMenu->Enable( false );
    m_exportMenu->Enable( false );
    m_bitmap->Clear();
    RefreshLabels();
}

void MainFrame::OnMenuExport( wxCommandEvent& WXUNUSED( event ) )
{
    const DicomFile& dfile = m_tree->GetFile();
    if( &dfile == NULL ) return;

    wxFileDialog* dialog = new
	wxFileDialog( this, wxT( "Choose a file" ), *wxEmptyString,
		      *wxEmptyString, wxT( "Text files (*.txt)|*.txt" ),
		      wxSAVE | wxOVERWRITE_PROMPT );

    if( dialog->ShowModal() == wxID_OK )
    {
	wxFileOutputStream file( dialog->GetPath() );
	if( file.Ok() )
	{
	    m_exportedTags.Rewind();
	    TagSet::TagID tag;
	    wxString str;

	    while( (tag = m_exportedTags.GetNext()) != TagSet::TAG__LAST )
	    {
		str = TagSet::GetTagID( tag ) + wxT( '=' )
		    + dfile.GetTagString( tag ) + wxT( LINE_BREAK );
		const wxCharBuffer data = str.mb_str( wxConvUTF8 );
		file.Write( data, strlen( data ) );
	    }
	}
	file.Close();
    }

    dialog->Destroy();
}

void MainFrame::OnMenuQuit( wxCommandEvent& WXUNUSED( event ) )
{
    Close();
}

void MainFrame::OnMenuDisplayedTags( wxCommandEvent& WXUNUSED( event ) )
{
    TagDialog dialog( this, -1, wxT( "Choose Displayed Tags" ) );
    dialog.SetTags( m_displayedTags );

    if( dialog.ShowModal() == wxID_OK )
    {
	dialog.GetTags( m_displayedTags );
	RefreshDisplayedTags();
	RefreshLabels();
    }
}

void MainFrame::OnMenuExportedTags( wxCommandEvent& WXUNUSED( event ) )
{
    TagDialog dialog( this, -1, wxT( "Choose Exported Tags" ) );
    dialog.SetTags( m_exportedTags );
    if( dialog.ShowModal() == wxID_OK ) dialog.GetTags( m_exportedTags );
}

void MainFrame::OnMenuAbout( wxCommandEvent& WXUNUSED( event ) )
{
    wxMessageBox( UniString( L"DicomSel " VERSION_STRING "\n"
			      "Copyright (C) 2005 IRCAD\n\n"
			      "Authors: Benjamin Gaillard, "
			      "Marc-Aur\u00E8le M\u00F6rk, "
			      "Guillaume Spitz" ), wxT( "About DicomSel" ),
		  wxOK | wxICON_INFORMATION, this );
}

void MainFrame::OnSelectionChanged( wxTreeEvent& WXUNUSED( event ) )
{
    const wxImage& image = m_tree->GetImage();
    m_bitmap->SetImage( image );
    RefreshLabels();

    m_exportMenu->Enable( image.Ok() );
}

} // namespace dicomsel

/* End of File */
