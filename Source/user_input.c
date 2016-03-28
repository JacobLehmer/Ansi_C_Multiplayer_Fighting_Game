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
unsigned char control_code = 0x00;

int is_shift = 0;
user_controls * control_info = (user_controls *) _thread_info;
while(killall != -1)
     {
          read(kbd_fd,&kbdev,sizeof(kbdev));
          if(kbdev.type != EV_KEY) continue;
          switch(kbdev.code)
               {
               case KEY_LEFTSHIFT: 
               {
                    if(kbdev.value == 1) is_shift = 1;
                    else is_shift = 0;
               }break;
               case KEY_W:
               {
                    if(kbdev.value == 1)
                    {
                         if(is_shift)control_info->command = 'W';
                         else control_info->command = 'w';
                         control_info->checked = 0;
                    }
               }break;
               case KEY_A:
               {
                    if(kbdev.value == 1)
                    {
                         if(is_shift)control_info->command = 'A';
                         else control_info->command = 'a';
                         control_info->checked = 0;
                    }
               }break; 
               case KEY_S:
               {
                    if(kbdev.value == 1)
                    {
                         if(is_shift)control_info->command = 'S';
                         else control_info->command = 's';
                         control_info->checked = 0;
                    }
               }break; 
               case KEY_D:
               {
                    if(kbdev.value == 1)
                    {
                         if(is_shift)control_info->command = 'D';
                         else control_info->command = 'd';
                         control_info->checked = 0;
                    }
               }break;
               case KEY_UP:
               {
                    if(kbdev.value == 1)
                    {
                         if(is_shift)control_info->command = ',';
                         else control_info->command = '<';
                         control_info->checked = 0;
                    }
               }break;
               }
               //TODO pollfd on read so that it can skip the read if it needs to or control code in the struct itself, possibly a byte of data containing the directions and movements
     }
     close(kbd_fd);
}
