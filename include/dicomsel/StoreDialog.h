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
//#ifndef WX_PRECOMP
# include <wx/dialog.h>
# include <wx/window.h>
//#endif // !WX_PRECOMP

// Current module
# include <wx/panel.h>
# include <wx/textctrl.h>
# include <wx/stattext.h>
# include <wx/notebook.h>
# include <wx/checkbox.h>
# include <wx/config.h>


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
    
    
    void SetPortRcp(wxString port);
    void SetPortSend(wxString port);
    void SetAddressSend(wxString ad);
    void IsConfigEnable();	
    bool IsCheck();
    bool IsCheckTimeFile();
    void SetCheck(bool v);  
    wxStaticText *fileName;


private: 
    wxPanel            *pToolsPanel;
    wxTextCtrl         *pEditPortRcp;
    wxTextCtrl         *pEditAddressSend;
    wxTextCtrl         *pEditPortSend;
    wxNotebook	       *notebook1;
    wxCheckBox         *m_checkBoxe;
    //wxCheckBox         *m_checkBoxeTls;	
    wxCheckBox	       *m_checkBoxe_tn;
    //wxCheckBox	       *m_checkBoxe_fe;
    wxConfig 	       *m_config;
    wxString	        m_fileName;


	Store *sendStore;


    // No copy/affectation
    StoreDialog( const StoreDialog& );
    StoreDialog& operator =( const StoreDialog& );


    void OnEnableConfig(wxCommandEvent &event);
    void OnEnableTimeFile(wxCommandEvent &event);
    void OnEnableExtention(wxCommandEvent &event);
    void OnSelectFile(wxCommandEvent &event);
    void OnSendFile(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
 



};

} // namespace dicomsel

#endif // DICOMSEL_STOREDIALOG_H

/* End of File */
