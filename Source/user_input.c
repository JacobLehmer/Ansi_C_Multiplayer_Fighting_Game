//Jacob Paul Lehmer (JPL) 3/18/16
//This is the class for handling the user controls
//AS of 3/18/16 this is currently simply reading stdin 
#include "Headers/include_all.h"

//=========-------------
//JPL 3/16/16 - This is the thread handler for the user controls
void * handle_user_controls(void * _thread_info)
{
user_controls * control_info = (user_controls *) _thread_info;
while(killall != -1)
     {
     control_info->command = getchar();
     control_info->checked = 0;
     }
}
