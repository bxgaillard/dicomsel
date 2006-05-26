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
#include <wx/dirdlg.h>

// System
#include <cstring>

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
    ID_BROWSE_BUTTON1,
    ID_BROWSE_BUTTON2,
    ID_SEND_BUTTON
};


/*
 * Event table
 */

BEGIN_EVENT_TABLE( StoreDialog, wxDialog )
    // Frame events
    EVT_CHECKBOX( ID_ENABLE_CONFIG, StoreDialog::OnEnableConfig )
    EVT_BUTTON( ID_BROWSE_BUTTON1, StoreDialog::OnSelectDirectory )
    EVT_BUTTON( ID_BROWSE_BUTTON2, StoreDialog::OnSelectFile )
    EVT_BUTTON( ID_SEND_BUTTON, StoreDialog::OnSendFile )
END_EVENT_TABLE()



StoreDialog::StoreDialog( wxWindow* parent, wxWindowID id, const wxString& title )
:   wxDialog( parent, id, title, wxDefaultPosition, wxDefaultSize,
	      wxDEFAULT_DIALOG_STYLE, wxT( "StoreDialogBox" ) )
{

	m_frmParent=parent;
	m_fileName="no_file";

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
	wxBoxSizer *PageReception_sizer1 = new wxBoxSizer(wxVERTICAL);


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
                               wxSize(50, 20));

         wxStaticText *rcp_text = new wxStaticText(PageReception,
                                             -1 ,
                                             wxT("reception path :"),
                                             wxDefaultPosition,
                                             wxSize(160,18));
	 m_rcp_path = new wxStaticText(PageReception,
                                             -1 ,
                                             wxT("./reception"),
                                             wxDefaultPosition,
                                             wxSize(160,50),wxST_NO_AUTORESIZE);//|wxTE_WORDWRAP

	m_button_rcp = new wxButton(PageReception,ID_BROWSE_BUTTON1, wxT( "Browse" ) );
	m_checkBoxe_tn = new wxCheckBox( PageReception, ID_ENABLE_TIMEFILE, wxT( "Generate filename from creation time" ) );
	m_checkBoxe_dr = new wxCheckBox( PageReception, ID_ENABLE_TIMEFILE, wxT( "Create a directory for each file" ) );
	//m_checkBoxe_fe = new wxCheckBox( PageReception, ID_ENABLE_EXTENTION," Generate filename with .dcm extention" );

	PageReception_sizer1->Add(m_checkBoxe, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 2);
	PageReception_sizer1->Add(stPortRcp, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 2);
	PageReception_sizer1->Add(pEditPortRcp, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 2);
	PageReception_sizer1->Add(m_checkBoxe_tn, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 2);
	PageReception_sizer1->Add(m_checkBoxe_dr, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 2);

	wxBoxSizer *PageReception_sizer2 = new wxBoxSizer(wxVERTICAL);
	PageReception_sizer2->Add(rcp_text, 0, wxTOP, 2);
	PageReception_sizer2->Add(m_rcp_path, 0, wxEXPAND ,4);


	wxBoxSizer *PageReception_sizer3 = new wxBoxSizer( wxHORIZONTAL );
	PageReception_sizer3->Add(PageReception_sizer2, 0,wxEXPAND|wxALL, 2);
	PageReception_sizer3->Add(m_button_rcp, 0,wxALIGN_RIGHT, 2);

	PageReception_sizer->Add(PageReception_sizer1,2,wxTOP);
	PageReception_sizer->Add(PageReception_sizer3,1,wxBOTTOM);

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
                               wxSize(50, 20));


	 wxButton *button2 = new wxButton(PageSending,ID_BROWSE_BUTTON2, wxT( "Browse" ) );
	 wxButton *buttonSend = new wxButton(PageSending,ID_SEND_BUTTON, wxT( "Send" ) );
         wxStaticText *path = new wxStaticText(PageSending,
                                             -1 ,
                                             wxT("File to send : "),
                                             wxDefaultPosition,
                                             wxDefaultSize);
	 fileName = new wxStaticText(PageSending,
                                             -1 ,
                                             wxT("no_file"),
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


wxString StoreDialog::GetPathRcp()
{
	wxString path = m_rcp_path-> GetLabel();
	return path;
}

wxString StoreDialog::GetFileName()
{
	return m_fileName;
}




void StoreDialog::SetPortRcp(wxString port)
{
	pEditPortRcp->SetValue(port);
}



void StoreDialog::SetPathRcp(wxString path)
{
	m_rcp_path->SetLabel(path);
}





bool StoreDialog::IsCheck(int box_number)
{
	bool result=true;
	if (box_number==1) result=(m_checkBoxe->GetValue());
	if (box_number==2) result= (m_checkBoxe_tn->GetValue());
	if (box_number==3) result=(m_checkBoxe_dr->GetValue());

return result;
}





void StoreDialog::SetCheck(bool box1,bool box2,bool box3)
{
	m_checkBoxe->SetValue(box1);
	m_checkBoxe_tn->SetValue(box2);
	m_checkBoxe_dr->SetValue(box3);
}



void StoreDialog::IsConfigEnable()
{
	if( m_checkBoxe->GetValue() )
	{
		 pEditPortRcp->Enable(true);
		 m_checkBoxe_tn->Enable(true);
		 m_checkBoxe_dr->Enable(true);
		 m_button_rcp->Enable(true);
	}

	else
	{
		 pEditPortRcp->Enable(false);
		 m_checkBoxe_tn->Enable(false);
		 m_checkBoxe_dr->Enable(false);
		 m_button_rcp->Enable(false);
	}



}

void StoreDialog::OnEnableConfig(wxCommandEvent& WXUNUSED( event ))
{
	IsConfigEnable();
}



void StoreDialog::OnSelectFile(wxCommandEvent& WXUNUSED( event ))
{

	// Pour le bouton browse : r�upere le nom du fichier s��tionn�
	wxFileDialog* openFileDialog = new wxFileDialog( this, wxT( "Open file" ), wxT( "" ), wxT( "" ), wxT( "Dicom files (*.dcm)|*.dcm|Dicom files (*.DCM)|*.DCM|All files (*.*)|*.*" ), wxOPEN, wxDefaultPosition );

	if (openFileDialog->ShowModal() == wxID_OK)
	{
		const wxString path( openFileDialog->GetPath() );
		wxString file=openFileDialog->GetFilename();
        	fileName->SetLabel(file);
		m_fileName=path;
		openFileDialog->Destroy();
	}

}


void StoreDialog::OnSelectDirectory(wxCommandEvent& WXUNUSED( event ))
{


 	 wxDirDialog *openDirDialog = new wxDirDialog
   	 (this,wxT( "Choose a directory" ),wxGetCwd(),0,wxDefaultPosition);

	if (openDirDialog->ShowModal() ==  wxID_OK)
	{
		const wxString path(openDirDialog->GetPath());
		m_rcp_path->SetLabel(path);
		m_path=path;
		openDirDialog->Destroy();
	}

}




void StoreDialog::OnSendFile(wxCommandEvent& WXUNUSED( event ))
{


	if (!std::strcmp("no_file",GetFileName().mb_str(wxConvUTF8)))
	{
		wxMessageBox( wxT( "Bad Filename" ), wxT( "DICOM Store Error" ),wxICON_HAND );
	}
	else
	{
		sendStore=new Store(GetAddressSend(),GetPortSend(),GetFileName(),2);
		sendStore->Run();
	}
}


} // namespace dicomsel

/* End of File */
