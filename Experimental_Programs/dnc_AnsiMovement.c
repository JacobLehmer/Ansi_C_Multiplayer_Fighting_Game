//Jacob Paul Lehmer(JPL) 3/14/16 
//This is a working experiment to figure out the controls for the system
//See the header file for more information
#include "dnc_AnsiMovement.h"

//=========-------------
//JPL 3/14/16
int main(int argc, char ** argv)
{
signal(SIGINT,control_c_catch);
struct termios new_t,old_t;
disable_echo(&new_t,&old_t);
     
     pthread_t timer_thread;
     pthread_t graphics_thread;
     pthread_t ui_thread;
     
     uint8_t tps = 10;
     
     object * items = malloc(sizeof(object)*6);
     
     initialize_items(items);
     
     graphics_threadinfo info_graphics =
     {
     .checked = 1,
     .player_index = 0,
     .size = 6,
     .in_game_objects = items
     };
     
     user_controls info_controls =
     {
     .checked = 1,
     .command = ' '
     };
     
     if(pthread_create(&timer_thread,NULL,game_timer,(void*)&tps ) <0)
     {
     perror("Thread Creation Failure");
     return 1;
     }
     
     if(pthread_create(&graphics_thread,NULL,graphics_handler,(void*)&info_graphics) < 0)
     {
     perror("Thread Creation Failure");
     return 1;
     }
     
     if(pthread_create(&ui_thread,NULL,handle_user_controls,(void*)&info_controls) < 0)
     {
     perror("Thread Creation Failure");
     return 1;
     }
     
     int timer_sync = game_sync;
     int attack_timer = 0;
     //THIS IS COMPLETELY AD HOC BELOW
     while(killall != -1)
     {
          if(timer_sync != game_sync && items[0].action == ATTACKING)
          {
          attack_timer ++;
          timer_sync = game_sync;
          }
     
          if(info_controls.checked != 1)
          {
               switch(info_controls.command)
               {
               case 'w' :
                    {
                    items[0].y_pos -=1;
                    items[0].direction = NORTH;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 'a' :    
                     {
                    items[0].x_pos -=1;
                    items[0].direction = WEST;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 's' :    
                     {
                    items[0].y_pos +=1;
                    items[0].direction = SOUTH;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 'd' :    
                     {
                    items[0].x_pos +=1;
                    items[0].direction = EAST;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 'W' :
                    {
                    items[0].y_pos -=1;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 'A' :    
                     {
                    items[0].x_pos -=1;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 'S' :    
                     {
                    items[0].y_pos +=1;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 'D' :    
                     {
                    items[0].x_pos +=1;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case ',' :
                    {
                    items[0].action = ATTACKING;
                    info_graphics.checked = 1;
                    break;
                    }
               case '<' :
                    {
                    items[0].action = ATTACKING;
                    info_graphics.checked = 1;
                    break;
                    }
               }
               info_controls.checked = 1;
          }
          
          if(attack_timer >=2)
          {
          items[0].action = IDLE;
          attack_timer = 0;
          info_graphics.checked = 1;
          }
          
     usleep(1000);
     }
     free(items);

reenable_echo(&old_t);
}


//=========-------------
//JPL 3/14/16 Utility Methods

//JPL 3/14/16 
void disable_echo(struct termios * new_t, struct termios * old_t)
{
     int fd = STDIN_FILENO;
     tcgetattr(fd,old_t);
     tcgetattr(fd,new_t);

     new_t->c_lflag &= ~ICANON;
     new_t->c_lflag &= ~ECHO;
     new_t->c_cc[VTIME] = 0;
     new_t->c_cc[VMIN]=1;   

     tcflush(fd,TCIFLUSH);
     tcsetattr(fd,TCSANOW,new_t);
}

//JPL 3/14/16
void reenable_echo(struct termios * old_t)
{
 tcsetattr(STDIN_FILENO,TCSANOW,old_t);
}

//=========-------------
//JPL 3/14/16 This is the graphics thread for the system
void * graphics_handler(void * _thread_info)
{
     struct winsize w;
     ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
     graphics_threadinfo * _info = (graphics_threadinfo * ) _thread_info;
     unsigned int scrn_col = w.ws_col;
     unsigned int scrn_row = w.ws_row;
     long int frames = 0;
     int sync_check = game_sync;
     int ref_x = _info->in_game_objects[_info->player_index].x_pos;
     int ref_y = _info->in_game_objects[_info->player_index].y_pos;
     printf("%d,%d\n",ref_x,ref_y);
     while(killall != -1)
          {
          if(sync_check != game_sync)
               {
               if(_info->checked != 0)
                    {
                    ref_x = _info->in_game_objects[_info->player_index].x_pos;
                    ref_y = _info->in_game_objects[_info->player_index].y_pos;
                    
                    draw_screen(_info,ref_x,ref_y,scrn_col,scrn_row);
                    _info->checked = 0;
                    sync_check = game_sync;
                    }
               }
          else
               {
               usleep(1000);
               }
          }

}
//=========-------------
//JPL 3/16/16 this will draw the screen around the player
void draw_screen(graphics_threadinfo * _info, int ref_x, int ref_y, unsigned int col, unsigned int row)
{
int rel_x = 0;
int rel_y = 0;
printf("\x1b[2J");
     for(int i =0; i < _info->size; i++)
     {
     rel_x = (ref_x - _info->in_game_objects[i].x_pos);
     rel_y = (ref_y - _info->in_game_objects[i].y_pos);
     if((rel_x*rel_x + rel_y*rel_y) <= 1000)
          {
          printf("\x1b[%d;%dH%s\x1b[1;1H",( row/2 - rel_y),(col/2 - rel_x*2),get_object(&(_info->in_game_objects[i])));
          }
     }
     
     fflush(stdout);

}

//JPL 3/16/16 - This will return the ansi string corresponding to type and color with all cursor movements being relative
char * get_object(object * obj)
{
     char * returnString = malloc(sizeof(char)*100);
     switch(obj->type)
     {
     case PLAYER :
          {
           sprintf(returnString,"\x1b[%dm    \x1b[B\x1b[4D    \x1b[A\x1b[m",obj->color);
               if(obj->action == IDLE)
                   {
          switch(obj->direction)
               {
               case NORTH: sprintf(returnString,"%s\x1b[47m  \x1b[m",returnString);break;
               case EAST: sprintf(returnString,"%s\x1b[2B\x1b[2D\x1b[47m  \x1b[m",returnString);break;
               case SOUTH: sprintf(returnString,"%s\x1b[B\x1b[6D\x1b[47m  \x1b[m",returnString);break;
               case WEST: sprintf(returnString, "%s\x1b[4D\x1b[A\x1b[47m  \x1b[m",returnString);break;
               }
                    }
               else if(obj->action == ATTACKING)
                    {
                    switch(obj->direction)
                         {
                         case NORTH: sprintf(returnString,"%s\x1b[47m  \x1b[2D\x1b[A  \x1b[2D\x1b[A  \x1b[m",returnString);break;
                         case EAST: sprintf(returnString,"%s\x1b[2B\x1b[2D\x1b[47m      \x1b[m",returnString);break;
                         case SOUTH: sprintf(returnString,"%s\x1b[B\x1b[6D\x1b[47m  \x1b[B\x1b[2D  \x1b[B\x1b[2D  \x1b[m",returnString);break;
                         case WEST: sprintf(returnString, "%s\x1b[8D\x1b[A\x1b[47m      \x1b[m",returnString);break;
                         }
                    }
               }break;
     case BLOCK : sprintf(returnString,"\x1b[%dm    \x1b[B\x1b[4D    \x1b[A\x1b[m",obj->color);break;
     }
     return returnString;
}

//=========-------------
//=========-------------

//JPL 3/14/16 This is the timer system thread it will use usleep to time the system as it gives adequate precision for a system, 
//TODO: a calibration system loaded into the operation so that it will rebalence the amount of time that was used in order to keep stablity over long periods
void * game_timer(void * _ticks_per_second)
{
uint8_t tps = *((uint8_t *)_ticks_per_second);
unsigned int wait_time = 1000000/tps;

while(killall != 1)
     {
     game_sync *= -1;
     usleep(wait_time);
     }
}
//=========-------------
//JPL 3/7/16 This is the control c catch so that it can end gracefully
void control_c_catch(int catch)
{
killall = -1;
}

//=========-------------
//JPL 3/16/16 this is the initialization method
void initialize_items(object * _items)
{
_items[0] = (object)
     {
     .type = PLAYER,
     .color = GREEN,
     .direction = NORTH,
     .action = IDLE,
     .x_pos = 3,
     .y_pos = 0
     };
     
_items[1] = (object)
     {
     .type = BLOCK,
     .color = BLUE,
     .x_pos = 10,
     .y_pos = 0
     };
     
_items[2] = (object)
     {
     .type = BLOCK,
     .color = BLUE,
     .x_pos = -10,
     .y_pos = 0
     };
     
_items[3] = (object)
     {
     .type = BLOCK,
     .color = BLUE,
     .x_pos = 0,
     .y_pos = 10
     };
     
_items[4] = (object)
     {
     .type = BLOCK,
     .color = BLUE,
     .x_pos = 0,
     .y_pos = -10
     };
_items[5] = (object)
     {
     .type = BLOCK,
     .color = BLUE,
     .x_pos = 0,
     .y_pos = 0
     };

}

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








