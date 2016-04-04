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
struct pollfd rfds =
{
.fd = kbd_fd,
.events = POLLIN
};
int timeout_msecs = 25;
int retval;

unsigned char control_code = 0x00;
unsigned char attack_code = 0x00;

int is_shift = 0;
int is_space = 0;
user_controls * control_info = (user_controls *) _thread_info;
while(killall != -1)
     {
          retval = poll(&rfds,1,timeout_msecs);
          if(retval)
          {
               read(kbd_fd,&kbdev,sizeof(kbdev));
               if(kbdev.type != EV_KEY) continue;
               switch(kbdev.code)
                    {
                    case KEY_SPACE:
                    {
                         if(kbdev.value != 0) is_space = 1;
                         else is_space = 0;
                    }break;
                    case KEY_LEFTSHIFT: 
                    {
                         if(kbdev.value != 0) is_shift = 1;
                         else is_shift = 0;
                    }break;                                                                  
                    case KEY_W:
                    {
                         if(kbdev.value != 0)
                         {
                              control_code |= 0x11;
                              if(is_shift)control_code &= SHIFT_MASK;
                              if(is_space)control_code &= SPACE_MASK;
                         }
                         else
                         {
                              control_code &= ~(0x11);
                         }
                    }break;
                    case KEY_A:
                    {
                         if(kbdev.value != 0)
                         {
                              control_code |= 0x22;
                              if(is_shift)control_code &= SHIFT_MASK;
                              if(is_space)control_code &= SPACE_MASK;
                         }
                         else
                         {
                              control_code &= ~(0x22);
                         }
                    }break; 
                    case KEY_S:
                    {
                         if(kbdev.value != 0)
                         {
                              control_code |= 0x44;
                              if(is_shift)control_code &= SHIFT_MASK;
                              if(is_space)control_code &= SPACE_MASK;
                         }
                         else
                         {
                             control_code &= ~(0x44);

                         }
                    }break; 
                    case KEY_D:
                    {
                         if(kbdev.value != 0)
                         {
                              control_code |= 0x88;
                              if(is_shift)control_code &= SHIFT_MASK;
                              if(is_space)control_code &= SPACE_MASK;
                         }
                         else
                         {
                           control_code &= ~(0x88);
                         }
                    }break;
                    case KEY_UP:
                    {
                         if(kbdev.value != 0)
                         {
                            attack_code |= 0x01;
                         }
                         else
                         {
                            attack_code &= ~(0x01);
                         }
                    }break;
                    }
               }
               
               control_info->attack = attack_code;
               
               if(control_code != 0x00)
               {
                control_info->command = control_code; 
                control_info->checked = 0;    
               }
               //TODO get attack to work again
     }
     close(kbd_fd);
}
