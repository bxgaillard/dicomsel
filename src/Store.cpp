#include <dicomsel/Storescp.h>
#include <dicomsel/Storescu.h>
#include <dicomsel/Store.h>



namespace dicomsel
{


BEGIN_EVENT_TABLE(Store, wxFrame)
    EVT_TIMER(wxID_ANY,         Store::OnTimer)
END_EVENT_TABLE()




// constructor for receive

Store::Store(wxString Port,MainFrame *frame_,int type_,bool timeFile_)
{
	unsigned long port_num;
	Port.ToULong(&port_num,10);
	unsigned int p=(unsigned int)port_num;
	Frame=frame_;
	Type=type_;
	Port_num=p;
	m_tmrRunning = new wxTimer(this); 
	opt_timeNames=timeFile_;
}


//constructor for send

Store::Store(wxString Address_,wxString Port_,wxString FileName_,int type_)
{

	Type=type_;
	Address=(char*)malloc(Address_.Len());
	strcpy( Address, (const char*)Address_.mb_str(wxConvUTF8) );
	Port=(char*)malloc(Port_.Len());
	strcpy( Port, (const char*)Port_.mb_str(wxConvUTF8) );
	FileName=(char*)malloc(FileName_.Len());
	strcpy( FileName, (const char*)FileName_.mb_str(wxConvUTF8) );
	m_tmrRunning = new wxTimer(this); 
}



void* Store::Entry()
{


if(Type==1)
{

	char *tab[ ] = { "storescp"};
	server(1,tab,Port_num,Frame);
}



if(Type==2)
{
	char *tab[ ] = { "storescu",Address,Port,FileName};	
	send(4,tab);
	m_blnFinished = true;
}

}

void Store::Run()
{
    if( IsRunning() ) {};                   // we are already doing something!

    m_blnFinished = false;                      // flag we are not done
    m_tmrRunning->Start(100);                   // start timer
    wxThreadHelper::Create();                   // create thread
    GetThread()->Run();                         // run.....
}



bool Store::IsRunning()
{
    wxThread *current_thread = wxThreadHelper::GetThread();
    if( current_thread )
    {
        if( current_thread->IsRunning() ) return true;
        else return false;
    }

    return false;
}

void Store::OnTimer(wxTimerEvent& WXUNUSED( event ))
{

if (Type ==1)
{ 
   if( m_blnFinished )
    {
        m_tmrRunning->Stop();
        wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED , 55);
        evt.SetEventObject(Frame);
        Frame->GetEventHandler()->ProcessEvent(evt);

    }
}


if (Type ==2)
{ 
   if( m_blnFinished && send_ok )
    {		m_tmrRunning->Stop();
		m_blnFinished = false;
		wxMessageBox("File sent : OK !","DicomeStore");

    }

  if( m_blnFinished && !send_ok )
  {		 m_tmrRunning->Stop(); 
		 wxMessageBox(send_error_msg,"DicomeStore");

  }   

}



}



} // end of namespace dicomsel

