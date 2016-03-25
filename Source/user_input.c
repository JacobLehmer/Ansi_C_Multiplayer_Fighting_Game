//Jacob Paul Lehmer (JPL) 3/18/16
//This is the class for handling the user controls
//AS of 3/18/16 this is currently simply reading stdin 
#include "Headers/include_all.h"

//=========-------------
//JPL 3/16/16 - This is the thread handler for the user controls
void * handle_user_controls(void * _thread_info)
{
int kbd_fd;
kbd_fd = open("/dev/input/event0",O_RDWR);
struct input_event kbdev;

int is_shift = 0;
user_controls * control_info = (user_controls *) _thread_info;
while(killall != -1)
     {
     read(kbd_fd,&kbdev,sizof(kbdev));
     if(kbdev.type != EV_KEY) continue;
     switch(kbdev.)
     control_info->command = getchar();
     control_info->checked = 0;
     }
}
