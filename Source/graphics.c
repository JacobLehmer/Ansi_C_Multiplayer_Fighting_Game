//Jacob Paul Lehmer (JPL) 3/18/16 
//This is the Graphics Portion of the game
#include "Headers/include_all.h"

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
     int sync_check = client_game_sync;
     int ref_x = _info->in_game_objects[_info->player_index].x_pos;
     int ref_y = _info->in_game_objects[_info->player_index].y_pos;
     //printf("%d,%d\n",ref_x,ref_y);
     while(killall != -1)
          {
          if(sync_check != client_game_sync)
               {
               if(_info->checked != 0)
                    {
                    ref_x = _info->in_game_objects[_info->player_index].x_pos;
                    ref_y = _info->in_game_objects[_info->player_index].y_pos;
                    
                    draw_screen(_info,ref_x,ref_y,scrn_col,scrn_row);
                    _info->checked = 0;
                    sync_check = client_game_sync;
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
     if(_info->in_game_objects[i].health == DEAD) continue;
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
//All of the object drawing information is stored here
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

