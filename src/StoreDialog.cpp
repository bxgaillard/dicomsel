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
#include <wx/dialog.h>
#include <wx/window.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/tabctrl.h>
#include <wx/textctrl.h>
#include <wx/config.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>

// Current module
#include <dicomsel/StoreDialog.h>
#include <dicomsel/Store.h>
#include <dicomsel/MainFrame.h>


namespace dicomsel
{

enum
{
    ID_EDIT_PORT,
    ID_EDIT_ADDRESS,
    ID_ENABLE_CONFIG,
    ID_ENABLE_TIMEFILE,
    ID_ENABLE_EXTENTION,
    ID_BROWSE_BUTTON,
    ID_SEND_BUTTON
};


/*
 * Event table
 */

BEGIN_EVENT_TABLE( StoreDialog, wxDialog )
    // Frame events
    EVT_CHECKBOX( ID_ENABLE_CONFIG, StoreDialog::OnEnableConfig )
//    EVT_CHECKBOX( ID_ENABLE_TIMEFILE, StoreDialog::OnEnableTimeFile )
//    EVT_CHECKBOX( ID_ENABLE_EXTENTION, StoreDialog::OnEnableExtention )
    EVT_BUTTON( ID_BROWSE_BUTTON, StoreDialog::OnSelectFile )
    EVT_BUTTON( ID_SEND_BUTTON, StoreDialog::OnSendFile )
END_EVENT_TABLE()



StoreDialog::StoreDialog( wxWindow* parent, wxWindowID id, const wxString& title )
:   wxDialog( parent, id, title, wxDefaultPosition, wxDefaultSize,
	      wxDEFAULT_DIALOG_STYLE, wxT( "StoreDialogBox" ) )
{
m_frmParent=parent;

notebook1 = new wxNotebook(this,
                                 wxID_ANY,
                                 wxDefaultPosition,
                                 wxDefaultSize);



wxPanel *PageReception = new wxPanel(notebook1,
                              -1,
                              wxDefaultPosition,
                              wxDefaultSize,
                              wxNO_BORDER);


wxPanel *PageSending = new wxPanel(notebook1, wxID_ANY);




notebook1->AddPage(PageReception, wxT( "Reception" ),TRUE);
notebook1->AddPage(PageSending, wxT( "Sending" ),FALSE);

/////RECEIVE
    wxBoxSizer *dialog_sizer = new wxBoxSizer(wxVERTICAL);
    dialog_sizer->Add(notebook1, 1, wxEXPAND|wxALL, 5);

    wxBoxSizer *PageReception_sizer = new wxBoxSizer(wxVERTICAL);


    m_checkBoxe = new wxCheckBox( PageReception, ID_ENABLE_CONFIG, wxT( "Active reception" ) );
 //PORT
	wxStaticText *stPortRcp = new wxStaticText(PageReception,
                                             -1 ,
                                             wxT("Port"),
                                             wxDefaultPosition,
                                             wxDefaultSize);

	pEditPortRcp = new wxTextCtrl(PageReception,
                               ID_EDIT_PORT,
                               wxT("4006"),
                               wxDefaultPosition,
                               wxSize(40, 20));

	//m_checkBoxeTls = new wxCheckBox( PageReception, ID_ENABLE_CONFIG,"Active Tls transmission" );
	m_checkBoxe_tn = new wxCheckBox( PageReception, ID_ENABLE_TIMEFILE, wxT( "Generate filename from creation time" ) );
	//m_checkBoxe_fe = new wxCheckBox( PageReception, ID_ENABLE_EXTENTION," Generate filename with .dcm extention" );

    PageReception_sizer->Add(m_checkBoxe, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 2);
    PageReception_sizer->Add(stPortRcp, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 2);
    PageReception_sizer->Add(pEditPortRcp, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 4);
    PageReception_sizer->Add(m_checkBoxe_tn, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 2);
    //PageReception_sizer->Add(m_checkBoxe_fe, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 2);



    PageReception->SetSizer(PageReception_sizer);
    PageReception->SetAutoLayout(true);



// SENDING

    wxBoxSizer *PageSending_sizer = new wxBoxSizer(wxVERTICAL);

//Adresses
	wxStaticText *pAddressSend = new wxStaticText(PageSending,
                                              -1 ,
                                              wxT("Addresse") ,
                                              wxDefaultPosition,
                                              wxDefaultSize);

	pEditAddressSend = new wxTextCtrl(PageSending,
                                  ID_EDIT_ADDRESS,
                                  wxT("192.168.0.1"),
                                  wxDefaultPosition,
                                  wxSize(110, 20));



//PORT
	wxStaticText *stPortSend = new wxStaticText(PageSending,
                                             -1 ,
                                             wxT("Port"),
                                             wxDefaultPosition,
                                             wxDefaultSize);

	pEditPortSend = new wxTextCtrl(PageSending,
                               ID_EDIT_PORT,
                               wxT("4006"),
                               wxDefaultPosition,
                               wxSize(40, 20));


	 wxButton *button2 = new wxButton(PageSending,ID_BROWSE_BUTTON, wxT( "Browse" ) );
	 wxButton *buttonSend = new wxButton(PageSending,ID_SEND_BUTTON, wxT( "Send" ) );
         wxStaticText *path = new wxStaticText(PageSending,
                                             -1 ,
                                             wxT("File to send : "),
                                             wxDefaultPosition,
                                             wxDefaultSize);
	 fileName = new wxStaticText(PageSending,
                                             -1 ,
                                             wxT("no file"),
                                             wxDefaultPosition,
                                             wxDefaultSize);



    PageSending_sizer->Add(pAddressSend, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 2);
    PageSending_sizer->Add(pEditAddressSend, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 4);
    PageSending_sizer->Add(stPortSend, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 2);
    PageSending_sizer->Add(pEditPortSend, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 4);
    PageSending_sizer->Add(button2, 0, wxALL | wxALIGN_CENTER, 4);
    PageSending_sizer->Add(buttonSend, 0, wxALL | wxALIGN_CENTER, 4);
    PageSending_sizer->Add(path, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 4);
    PageSending_sizer->Add(fileName, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 4);




    PageSending->SetSizer(PageSending_sizer);
    PageSending->SetAutoLayout(true);


    SetSizer(dialog_sizer);
    Fit();

  // Création du sizer principal
  wxBoxSizer *Psizer = new wxBoxSizer( wxVERTICAL );

   wxStaticBox* const box =new wxStaticBox( this, -1, wxT( "Network options" ) );
   wxStaticBoxSizer* const boxSizer = new wxStaticBoxSizer( box, wxVERTICAL );
   boxSizer->Add( notebook1, 1, wxALL | wxEXPAND, 4 );



  //boxSizer->Add(sizer1,1,wxALIGN_RIGHT);
  boxSizer->Add(CreateStdDialogButtonSizer( wxOK | wxCANCEL ), 0,
	wxALL | wxEXPAND, 4 );

  Psizer->Add( boxSizer, 1, wxALL | wxEXPAND, 4 );

  SetSizer(Psizer);


  Fit();

}


wxString StoreDialog::GetAddressSend()
{
  wxString ad = pEditAddressSend->GetValue();
  return ad;
}


wxString StoreDialog::GetPortSend()
{
  wxString port = pEditPortSend->GetValue();
  return port;
}


void StoreDialog::SetAddressSend(wxString ad)
{
  pEditAddressSend->SetValue(ad);

}


void StoreDialog::SetPortSend(wxString port)
{
  pEditPortSend->SetValue(port);

}



// Rcp
wxString StoreDialog::GetPortRcp()
{
  wxString port = pEditPortRcp->GetValue();
  return port;
}

wxString StoreDialog::GetFileName()
{
  return m_fileName;
}




void StoreDialog::SetPortRcp(wxString port)
{
  pEditPortRcp->SetValue(port);

}




bool StoreDialog::IsCheck()
{
  return (m_checkBoxe->GetValue());

}


bool StoreDialog::IsCheckTimeFile()
{
  return (m_checkBoxe_tn->GetValue());

}


void StoreDialog::SetCheck(bool v)
{
	m_checkBoxe->SetValue(v);
}



void StoreDialog::IsConfigEnable()
{
	if( m_checkBoxe->GetValue() )
	{
		 pEditPortRcp->Enable(true);
		 m_checkBoxe_tn->Enable(true);
		 //m_checkBoxe_fe->Enable(true);
	}

	else
	{
		 pEditPortRcp->Enable(false);
		 m_checkBoxe_tn->Enable(false);
		 //m_checkBoxe_fe->Enable(false);
	}



}

void StoreDialog::OnEnableConfig(wxCommandEvent& WXUNUSED( event ))
{
	IsConfigEnable();
}



void StoreDialog::OnSelectFile(wxCommandEvent& WXUNUSED( event ))
{

	// Pour le bouton browse : r�upere le nom du fichier s��tionn�
	wxFileDialog* openFileDialog = new wxFileDialog( this, wxT( "Open file" ), wxT( "" ), wxT( "" ), wxT( "Dicom files (*.dcm)|*.dcm" ), wxOPEN, wxDefaultPosition );

	if (openFileDialog->ShowModal() == wxID_OK)
	{
		const wxString path( openFileDialog->GetPath() );
		wxString file=openFileDialog->GetFilename();
        	fileName->SetLabel(file);
		m_fileName=path;
		openFileDialog->Destroy();
	}

}




void StoreDialog::OnSendFile(wxCommandEvent& WXUNUSED( event ))
{
	sendStore=new Store(GetAddressSend(),GetPortSend(),GetFileName(),2);
	sendStore->Run();


}


} // namespace dicomsel

/* End of File */
