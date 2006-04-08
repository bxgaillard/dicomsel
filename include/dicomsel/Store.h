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


#ifndef DICOMSEL_STORE_H
#define DICOMSEL_STORE_H
# include <wx/string.h>
#include <wx/thread.h>
#include <wx/timer.h>
#include <dicomsel/MainFrame.h>



namespace dicomsel
{

class MainFrame;


class Store: public wxFrame, public wxThreadHelper
{
private:
	wxTimer     *m_tmrRunning;
	bool        m_blnCancelled;
	int Type;
	char *Address;
	char *Port;
	char *FileName;
	unsigned int Port_num;
	MainFrame *Frame;

public:
	Store(wxString Port,MainFrame *frame_,int type,bool timeFile);
	Store(wxString Address_,wxString Port_,wxString FileName_, int type_ );
	//virtual _~Store( void );
	void        Run();
	void        *Entry();
	bool        IsRunning(); 
	void        OnTimer(wxTimerEvent &event);

DECLARE_EVENT_TABLE()

};



} // namespace dicomsel


#endif // DICOMSEL_STORE_H
