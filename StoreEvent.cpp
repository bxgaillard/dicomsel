
#include <dicomsel/StoreEvent.h>


BEGIN_EVENT_TABLE(wxBitmapCheckBox,wxControl)
        EVT_LEFT_DOWN   (wxBitmapCheckBox::OnMouse)
        EVT_LEFT_UP     (wxBitmapCheckBox::OnMouse)
    EVT_MOTION      (wxBitmapCheckBox::OnMouse)
END_EVENT_TABLE()

/***************************************************/


DEFINE_EVENT_TYPE(wxEVT_BMPCHECKBOX_CLICKED)

void wxBitmapCheckBox::OnMouse(wxMouseEvent &event){

        if(event.LeftDown()){
                SetValue(!checked);
               
                StoreEvent ev(wxEVT_BMPCHECKBOX_CLICKED,GetId());
                ev.SetEventObject(this);
                ev.setState(IsChecked());
                GetEventHandler()->ProcessEvent( ev );
               
        }
       
       
}
