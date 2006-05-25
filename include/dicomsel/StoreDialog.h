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


#ifndef DICOMSEL_STOREDIALOG_H
#define DICOMSEL_STOREDIALOG_H

// wxWidgets
#include <wx/dialog.h>
#include <wx/window.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/notebook.h>
#include <wx/checkbox.h>
#include <wx/config.h>

// Current module
#include <dicomsel/Store.h>

namespace dicomsel
{

class StoreDialog : public wxDialog
{
public:
    StoreDialog( wxWindow* parent, wxWindowID id, const wxString& title );
    //virtual ~StoreDialog( void );

    wxWindow    *m_frmParent;
    wxString GetAddressSend();
    wxString GetPortSend();
    wxString GetPortRcp();
    wxString GetFileName();
    wxString GetPathRcp();

    void SetPortRcp(wxString port);
    void SetPathRcp(wxString path);
    void SetPortSend(wxString port);
    void SetAddressSend(wxString ad);
    void IsConfigEnable();
    bool IsCheck(int box_number);
    void SetCheck(bool box1,bool box2,bool box3);
    wxStaticText *fileName;
    wxStaticText *m_rcp_path;



private:
    wxPanel            *pToolsPanel;
    wxTextCtrl         *pEditPortRcp;
    wxTextCtrl         *pEditAddressSend;
    wxTextCtrl         *pEditPortSend;
    wxNotebook	       *notebook1;
    wxCheckBox         *m_checkBoxe;
    wxCheckBox	       *m_checkBoxe_tn;
    wxCheckBox	       *m_checkBoxe_dr;
    wxConfig 	       *m_config;
    wxString	        m_fileName; 
    wxString	        m_path;

	wxButton * m_button_rcp;
	Store *sendStore;


    // No copy/affectation
    StoreDialog( const StoreDialog& );
    StoreDialog& operator =( const StoreDialog& );


    void OnEnableConfig(wxCommandEvent &event);
    void OnEnableTimeFile(wxCommandEvent &event);
    void OnEnableExtention(wxCommandEvent &event);
    void OnSelectFile(wxCommandEvent &event);
    void OnSelectDirectory(wxCommandEvent &event);
    void OnSendFile(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()




};

} // namespace dicomsel

#endif // DICOMSEL_STOREDIALOG_H

/* End of File */
