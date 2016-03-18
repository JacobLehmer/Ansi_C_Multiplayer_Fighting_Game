//Jacob Paul Lehmer (JPL) 3/18/16
//This is the main file for the operation fo the ansi fighting game

#include "Headers/include_all.h"

//JPL 3/14/16 - The main method, in addition to setting up the system will also be acting as the game engine
int main(int argc, char ** argv)
{
killall =0;
game_sync = -1;
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
     //THIS IS COMPLETELY AD HOC BELOW AND NOT MEANT TO BE USED IN FURTHER PROJECTS MORE THOUGHT NEEDS TO BE PUT INTO THE SYSTEM HERE
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
//JPL 3/7/16 This is the control c catch so that it can end gracefully
void control_c_catch(int catch)
{
killall = -1;
}

//=========-------------
//JPL 3/16/16 this is the initialization method
//NOTE: This is currently hardcoded and needs to be rethought
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


