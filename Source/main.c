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
     int just_hit = 0;
     int movement_check = 1;
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
          movement_check =1;
               switch(info_controls.command)
               {
               case 'w' :
                    {
                    
                    for(int i =1; i< 6;i++) 
                    {
                    if(items[i].health <= DEAD) continue;
                    movement_check *= (((items[i].y_pos - (items[0].y_pos-1))*(items[i].y_pos - (items[0].y_pos-1)) + (items[i].x_pos - items[0].x_pos)*(items[i].x_pos - items[0].x_pos)) > 2);
                    }
                    if(movement_check == 1)items[0].y_pos -=1;
                    
                    items[0].direction = NORTH;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    just_hit = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 'a' :    
                     {
                    for(int i =1; i< 6;i++) 
                    {
                    if(items[i].health <= DEAD) continue;
                    movement_check *=(((items[i].y_pos - (items[0].y_pos))*(items[i].y_pos - (items[0].y_pos)) + (items[i].x_pos - (items[0].x_pos-1))*(items[i].x_pos - (items[0].x_pos-1))) > 2);
                    }
                    if(movement_check == 1)items[0].x_pos -=1;
                    items[0].direction = WEST;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    just_hit = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 's' :    
                     {
                    for(int i =1; i< 6;i++) 
                    {
                    if(items[i].health <= DEAD) continue;
                    movement_check *=(((items[i].y_pos - (items[0].y_pos+1))*(items[i].y_pos - (items[0].y_pos+1)) + (items[i].x_pos - items[0].x_pos)*(items[i].x_pos - items[0].x_pos)) > 2);
                    }
                    if(movement_check == 1)items[0].y_pos +=1;
                    items[0].direction = SOUTH;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    just_hit = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 'd' :    
                     {
                    for(int i =1; i< 6;i++)
                    {
                    if(items[i].health <= DEAD) continue;
                    movement_check *=(((items[i].y_pos - (items[0].y_pos))*(items[i].y_pos - (items[0].y_pos)) + (items[i].x_pos - (items[0].x_pos+1))*(items[i].x_pos - (items[0].x_pos+1))) > 2);
                    }
                    if(movement_check == 1)items[0].x_pos +=1;
                    items[0].direction = EAST;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    just_hit = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 'W' :
                    {
                    for(int i =1; i< 6;i++)
                    {
                    if(items[i].health <= DEAD) continue;
                    movement_check *=(((items[i].y_pos - (items[0].y_pos-1))*(items[i].y_pos - (items[0].y_pos-1)) + (items[i].x_pos - items[0].x_pos)*(items[i].x_pos - items[0].x_pos)) > 2);
                    }
                    if(movement_check == 1)items[0].y_pos -=1;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    just_hit = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 'A' :    
                     {
                    for(int i =1; i< 6;i++) 
                    {
                    if(items[i].health <= DEAD) continue;
                    movement_check *=(((items[i].y_pos - (items[0].y_pos))*(items[i].y_pos - (items[0].y_pos)) + (items[i].x_pos - (items[0].x_pos-1))*(items[i].x_pos - (items[0].x_pos-1))) > 2);
                    }
                    if(movement_check == 1)items[0].x_pos -=1;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    just_hit = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 'S' :    
                     {
                    for(int i =1; i< 6;i++)
                    {
                    if(items[i].health <= DEAD) continue;
                    movement_check *=(((items[i].y_pos - (items[0].y_pos+1))*(items[i].y_pos - (items[0].y_pos+1)) + (items[i].x_pos - items[0].x_pos)*(items[i].x_pos - items[0].x_pos)) > 2);
                    }
                    if(movement_check == 1)items[0].y_pos +=1;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    just_hit = 0;
                    info_graphics.checked = 1;
                    break;
                    }
               case 'D' :    
                     {
                    for(int i =1; i< 6;i++)
                    {
                    if(items[i].health <= DEAD) continue; 
                    movement_check *=(((items[i].y_pos - (items[0].y_pos))*(items[i].y_pos - (items[0].y_pos)) + (items[i].x_pos - (items[0].x_pos+1))*(items[i].x_pos - (items[0].x_pos+1))) > 2);
                    }
                    if(movement_check == 1)items[0].x_pos +=1;
                    items[0].action = IDLE;
                    attack_timer = 0;
                    just_hit = 0;
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
          just_hit = 0;
          info_graphics.checked = 1;
          }
          
          if(attack_timer ==1 && just_hit == 0)
          {
          just_hit = 1;
          switch(items[0].direction)
               {
                case NORTH:
                    {
                    for(int i =1; i< 6;i++) 
                         {
                         if(items[i].health <= DEAD) continue;
                         if(((items[i].y_pos - (items[0].y_pos-2))*(items[i].y_pos - (items[0].y_pos-2)) + (items[i].x_pos - items[0].x_pos)*(items[i].x_pos - items[0].x_pos)) <= 2)
                              {
                              if(items[i].health < items[0].damage) items[i].health =0;
                              else items[i].health -= items[0].damage; 
                              }
                         }
                    };break; 
               case EAST:
                    {
                    for(int i =1; i< 6;i++) 
                         {
                         if(items[i].health <= DEAD) continue;
                         if(((items[i].y_pos - (items[0].y_pos))*(items[i].y_pos - (items[0].y_pos)) + (items[i].x_pos - (items[0].x_pos+2))*(items[i].x_pos - (items[0].x_pos+2))) <= 2)
                              {
                              if(items[i].health < items[0].damage) items[i].health =0;
                              else items[i].health -= items[0].damage; 
                              }
                         }
                    };break; 
               
               case SOUTH:
                    {
                    for(int i =1; i< 6;i++) 
                         {
                         if(items[i].health <= DEAD) continue;
                         if(((items[i].y_pos - (items[0].y_pos+2))*(items[i].y_pos - (items[0].y_pos+2)) + (items[i].x_pos - items[0].x_pos)*(items[i].x_pos - items[0].x_pos)) <=2)
                              {
                              if(items[i].health < items[0].damage) items[i].health =0;
                              else items[i].health -= items[0].damage; 
                              }
                         }
                    };break;
               case WEST:
                    {
                    for(int i =1; i< 6;i++) 
                         {
                         if(items[i].health <= DEAD) continue;
                         if(((items[i].y_pos - (items[0].y_pos))*(items[i].y_pos - (items[0].y_pos)) + (items[i].x_pos - (items[0].x_pos-2))*(items[i].x_pos - (items[0].x_pos-2))) <= 2)
                              {
                              if(items[i].health < items[0].damage) items[i].health =0;
                              else items[i].health -= items[0].damage; 
                              }
                         }
                    };break;
                    
               }
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
     .y_pos = 0,
     .health = 100,
     .damage = 50
     };
     
_items[1] = (object)
     {
     .type = BLOCK,
     .color = BLUE,
     .x_pos = 10,
     .y_pos = 0,
     .health = 100
     };
     
_items[2] = (object)
     {
     .type = BLOCK,
     .color = BLUE,
     .x_pos = -10,
     .y_pos = 0,
     .health = 100
     };
     
_items[3] = (object)
     {
     .type = BLOCK,
     .color = BLUE,
     .x_pos = 0,
     .y_pos = 10,
     .health = 100
     };
     
_items[4] = (object)
     {
     .type = BLOCK,
     .color = BLUE,
     .x_pos = 0,
     .y_pos = -10,
     .health = 100
     };
_items[5] = (object)
     {
     .type = BLOCK,
     .color = BLUE,
     .x_pos = 0,
     .y_pos = 0,
     .health = 100
     };

}
/*
//=========-------------
//JPL 3/18/16 this is the method that will build the collision detection system
collision_detection_storage * build_collision_detection_storage(object * items, int size)
{
collision_detection_storage * game_map = malloc(sizeof(collision_detection_storage));

//this part sets the bounds for the two arrays
int size_of_x = 0; //this corresponds to the total number of unique x positions
int * temp_x_array = malloc(sizeof(int)*size); //these two arrays are parallel, this will contain all of the unique x positions in the game map
int * temp_y_array = malloc(sizeof(int)*size); //this corresponds to the number of extra y entries associated with the x value in the array
int position;
for(int i =0;i< size; i++)
     {
     position = binary_search(temp_x_array,size,items[i].x_pos);
     if(position == -1) 
          {
          size_of_x++;
          temp_x_array = insert_in_order(temp_x_array,size_of_x,size,items[i].x_pos);
          }
     else
          {
          temp_y_array[position] ++;
          }
     }
     
//this will build the actual ragged array 
game_map->in_game_objects = malloc(sizeof(object *)*size_of_x);

game_map->number_of_rows = size_of_x;

for(int i = 0;i< size_of_x;i++)
     {
     game_map->in_game_objects[i] = malloc(sizeof(x_case));
     game_map->in_game_objects[i].x = temp_x_array[i];
     }
     
     //this will install all of the actual objects and the y values for the objects
     
for(int i = 0;i< size;i++)
     {
     int obj_position = binary_search_map(game_map.in_game_objects, size_of_x,items[i].x_pos);
     game_map->in_game_objects[obj_position].number_of_ys = temp_y_array[obj_position] + 1;
     //need another binary search to traverse through the various levels
     }

}
*/
//=========-------------
//JPL 3/24/16 This is a nice binary search
int binary_search(int * array, int array_size, int item)
{

int search_length = array_size -1;
search_length ++;
search_length /=2;
int current_index = search_length;
search_length ++;
search_length /=2;
while(1)
     {
     if(item > array[current_index]) current_index += search_length;
     else if(item < array[current_index]) current_index -= search_length;
     else break;

     if(current_index < 0 || current_index > array_size-1 ) return -1;
     search_length ++;
     search_length  /= 2;
     }

     return current_index;
}

//=========-------------
//JPL 3/24/16 This is a binary search that accepts an initial and final index
int binary_search_init_pos(int * array, int start, int end, int item)
{
int array_size = (end - start);
int search_length = (end - start) ;
search_length ++;
search_length /=2;
int current_index = search_length + start;
search_length ++;
search_length /=2;
while(1)
     {
     if(item > array[current_index]) current_index += search_length;
     else if(item < array[current_index]) current_index -= search_length;
     else break;

     if(current_index < 0 || current_index > array_size ) return -1;
     search_length ++;
     search_length  /= 2;
     }

     return current_index;
}

//JPL 3/24/16 This is a adjacency binary search that will return a position that the current location is bigger and the previous location is smaller
//returns -1 if element is in array
int adjacency_binary_search(int * array, int start, int end, int item)
{
int array_size = (end - start);
int search_length = (end - start) ;
search_length ++;
search_length /=2;
int current_index = search_length + start;
if((array[current_index-1] < item && array[current_index] > item)) return current_index;
search_length ++;
search_length /=2;

while(1)
     {
     if(item > array[current_index] && current_index != array_size) current_index += search_length;
     else if(item < array[current_index - 1] && current_index != 0) current_index -= search_length;
     else break;
     
     if(item == array[current_index-1]) return -1;
     search_length ++;
     search_length  /= 2;
     }
     return current_index;
}
/*
//JPL 3/24/16 This is a binary search to traverse the two dimensional array 
int binary_search_map(x_case * game_map, int size_of_x, int value)
{
int search_length = size_of_x -1;
search_length ++;
search_length /=2;
int current_index = search_length;
search_length ++;
search_length /=2;
while(1)
     {
     if(value > game_map[current_index].x) current_index += search_length;
     else if(value < game_map[current_index].x) current_index -= search_length;
     else break;
     
     if(search_length == 1) return -1;
     if(search_length == 0) return -1;
     if(current_index < 0 || current_index > size_of_x-1 ) return -1;
     search_length ++;
     search_length  /= 2;
     }

     return current_index;
}
*/
//=========-------------
//JPL 3/24/16 This is a very nice copy and double method for pointer array expansion
void * copy_and_double(void * to_copy,int item_size, int array_size)
{
unsigned char * return_pointer = malloc(item_size*array_size*2);

for(int i = 0; i< array_size;i++)
     {
     return_pointer[i*item_size] = ((unsigned char*)to_copy)[i*item_size];
     }
return (void *) return_pointer;
}
/*
//=========-------------
//JPL 3/24/16 This is the insert in order function
int * insert_in_order(int * array,int current_array_size, int max_array_size, int value)
{
printf("insert_value:%d\n",value);

if(current_array_size >= max_array_size) array = copy_and_double(array,sizeof(int),current_array_size);

int insert_position = adjacency_binary_search(array, 0,current_array_size,value);
if(insert_position == -1) return NULL;
for(int i =0; i < (current_array_size - insert_position); i++)
     {
     array[(current_array_size-i)] = array[(current_array_size-1-i)];
     }
     array[insert_position] = value;
     return array;
}*/
