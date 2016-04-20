//Jacob Paul Lehmer (JPL) 3/18/16
//This is the main file for the operation fo the ansi fighting game

#include "Headers/include_all.h"

//JPL 3/14/16 - The main method, in addition to setting up the system will also be acting as the game engine
int main(int argc, char ** argv)
{

srand(time(NULL));
killall =0;
game_sync = -1;


struct termios new_t,old_t;
disable_echo(&new_t,&old_t);
     
     //TODO need to add another thread for communication
     //TODO Need to add in the functionality to make the host and client different
     //needs to decide connection or host before moving on
     ////Assuming host and proceeding with moving things around and building code
     
     settings game_settings = parse_command_line_options(argc,argv);
     
     //temp
     pthread_t communication_thread;
     //temp
     
     pthread_t timer_thread;
     pthread_t graphics_thread;
     pthread_t ui_thread;
     
     uint8_t tps = 10; //goes to communications

     
     //object * items = initialize_map(&elements, "map"); //goes to communications, or keep like this and have the loading
     
     //communication_controls info_communicaiton =
     //{
     //.port = 0;
     //.map_filename = "map";
     //};
     
     graphics_threadinfo info_graphics =
     {
     .checked = 1,
     .player_index = 0,
     .size = elements,
     .in_game_objects = items
     };
     
     user_controls info_controls =
     {
     .checked = 1,
     .command = ' ',
     .event_filename = find_used_device()
     };
     
     signal(SIGINT,control_c_catch);
     
     //temp
     if(pthread_create(&communication_thread,NULL, server_controls,(void*)&game_settings) < 0)
     {
     perror("Server Thread Creation Failure");
     return 1;
     }
     //temp
     
     
     if(pthread_create(&timer_thread,NULL,game_timer,(void*)&tps ) <0)
     {
     perror("Timer Thread Creation Failure");
     return 1;
     }
     
     if(pthread_create(&graphics_thread,NULL,graphics_handler,(void*)&info_graphics) < 0)
     {
     perror("Graphics Thread Creation Failure");
     return 1;
     }
     
     if(pthread_create(&ui_thread,NULL,handle_user_controls,(void*)&info_controls) < 0)
     {
     perror("User Control Thread Creation Failure");
     return 1;
     }
     
     //TODO contain this in arbitrary client protocol
     int socket = -1;
     if(game_settings.is_host == 0)
     {
          socket = connect_to_server(game_settings.server_address, game_settings.port);
          if(socket == -1)
          {
               perror("Connection Error");
               return 1;
          } 
     }
     
     startup_packet game_info = get_startup_packet(game_settings.is_host, socket);
     object * items = get_object_list_from_server(game_settings.is_host, socket);
     
     int timer_sync = game_sync;
     int attack_timer = 0;
     int just_hit = 0;
     int movement_check = 1;
     
     int elements = game_info.number_of_elements;

     int player_index = game_info.player_index;
     
     while(killall != -1)
     {
          if(timer_sync != game_sync && items[0].action == ATTACKING)
          {
          attack_timer ++;
          timer_sync = game_sync;
          }
     
          if(info_controls.checked != 1)
          {
               update_player(info_controls.command,player_index, items,elements);  
               info_graphics.checked = 1;   
               info_controls.checked = 1;
          }
          
          if(info_controls.attack ==0x01)
               {
               items[0].action = ATTACKING;
               info_graphics.checked = 1;
               }          
          
          if(attack_timer >=2)
          {
          items[0].action = IDLE;
          attack_timer = 0;
          just_hit = 0;
          info_graphics.checked = 1;
          }
          
          send_player_state(game_settings.is_host,items[player_index]);
          update_player_locations(game_settings.is_host,socket,items,game_info);
          
          //TODO this part needs to be done by the server
          
          if(attack_timer ==1 && just_hit == 0)
          {
          just_hit = 1;
          switch(items[0].direction)
               {
                case NORTH:
                    {
                    for(int i =1; i< elements;i++) 
                         {
                         if(items[i].health <= DEAD) continue;
                         if(((items[i].y_pos - (items[0].y_pos-2))*(items[i].y_pos - (items[0].y_pos-2)) + (items[i].x_pos - items[0].x_pos)*(items[i].x_pos - items[0].x_pos)) <= 2)
                              {
                              if(items[i].health < items[0].damage) items[i].health =DEAD;
                              else items[i].health -= items[0].damage; 
                              }
                         }
                    };break; 
               case EAST:
                    {
                    for(int i =1; i< elements;i++) 
                         {
                         if(items[i].health <= DEAD) continue;
                         if(((items[i].y_pos - (items[0].y_pos))*(items[i].y_pos - (items[0].y_pos)) + (items[i].x_pos - (items[0].x_pos+2))*(items[i].x_pos - (items[0].x_pos+2))) <= 2)
                              {
                              if(items[i].health < items[0].damage) items[i].health =DEAD;
                              else items[i].health -= items[0].damage; 
                              }
                         }
                    };break; 
               
               case SOUTH:
                    {
                    for(int i =1; i< elements;i++) 
                         {
                         if(items[i].health <= DEAD) continue;
                         if(((items[i].y_pos - (items[0].y_pos+2))*(items[i].y_pos - (items[0].y_pos+2)) + (items[i].x_pos - items[0].x_pos)*(items[i].x_pos - items[0].x_pos)) <=2)
                              {
                              if(items[i].health < items[0].damage) items[i].health =DEAD;
                              else items[i].health -= items[0].damage; 
                              }
                         }
                    };break;
               case WEST:
                    {
                    for(int i =1; i< elements;i++) 
                         {
                         if(items[i].health <= DEAD) continue;
                         if(((items[i].y_pos - (items[0].y_pos))*(items[i].y_pos - (items[0].y_pos)) + (items[i].x_pos - (items[0].x_pos-2))*(items[i].x_pos - (items[0].x_pos-2))) <= 2)
                              {
                              if(items[i].health < items[0].damage) items[i].health =DEAD;
                              else items[i].health -= items[0].damage; 
                              }
                         }
                    };break;
                    
               }
          }
          
          
     usleep(1000);
     }
     
     //Below this is fine
     
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
     .health = INVINCIBLE
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
     .health = INVINCIBLE
     };
     
_items[4] = (object)
     {
     .type = BLOCK,
     .color = BLUE,
     .x_pos = 0,
     .y_pos = -10,
     .health = INVINCIBLE
     };
_items[5] = (object)
     {
     .type = BLOCK,
     .color = BLUE,
     .x_pos = 0,
     .y_pos = 0,
     .health = INVINCIBLE
     };

}

//=========-------------
//JPL 4/4/16 This is the map initializer 
//@ represents a player spawn point
//+ represents 0,0
//* represents an invincible blue wall
// a space represents a space
object * initialize_map(int * size, char * filename, int number_of_players)
{
     int map_fd = open(filename, O_RDWR);
     int loop_size = 0;
     int start_stream_position = 0;
     int startx = 0;
     int starty = 0;
     int lines = 0;
     int elements_since_newline = 0;
     char element = 0x00;
     int object_at = number_of_players;
     int location_at = 0;
     (*size)=number_of_players;
     int starting_locations = 0;// this will correspond to the number of starting locatons stored on two parallel arrays which will be declared after this value is found
     
     object * return_ptr;
     if(map_fd == -1) 
     {
          printf("Failure to open file\n");  
          killall = -1;
          return NULL;
     }
     lseek(map_fd,0,SEEK_END);
     loop_size = lseek(map_fd,0,SEEK_CUR);
     lseek(map_fd,0,SEEK_SET);
     
     for(int i =0;i< loop_size;i++)
     {
          read(map_fd,&element,1);
          if(element == '+')
          {
          (*size) ++;
          startx = elements_since_newline;
          starty = lines;
          start_stream_position = lseek(map_fd,0,SEEK_CUR);
          elements_since_newline++;
          }
          else if(element == '\n')
          {
          lines++;
          elements_since_newline = 0;
          }
          else if(element == '*')
          {
          (*size)++;
          elements_since_newline++;
          }
          else if(element == ' ')
          {
          elements_since_newline++;
          }
          else if(element == '@')
          {
          starting_locations ++;
          elements_since_newline++;
          }
     }
     
     lines = 0;
     elements_since_newline = 0;
     element = 0x00;
     
     return_ptr = malloc(sizeof(object)*(*size));
     
     int start_locations_x[starting_locations];
     int start_locations_y[starting_locations];
     

     
     lseek(map_fd,0,SEEK_SET);
     
     for(int i =0;i< loop_size;i++)
     {
          read(map_fd,&element,1);
          if(element == '+')
          {
          elements_since_newline++;
          }
          else if(element == '@')
          {
          start_locations_x[location_at] = 2*(elements_since_newline - startx);
          start_locations_y[location_at] = 2*(lines - starty);
          location_at ++;
          elements_since_newline++;
          }
          else if(element == '\n')
          {
          lines++;
          elements_since_newline = 0;
          }
          else if(element == '*')
          {
          return_ptr[object_at] = (object)
          {
          .type = BLOCK,
          .color = BLUE,
          .x_pos = 2*(elements_since_newline - startx),
          .y_pos = 2*(lines - starty),
          .health = INVINCIBLE
          };
          object_at ++;
          elements_since_newline++;
          }
          else if(element == ' ')
          {
          elements_since_newline++;
          }
     }
     int guess = 0;
     for(int i =0;i< number_of_players;i++)
     {
     guess = rand()%starting_locations;

     return_ptr[i] = (object)
     {
     .type = PLAYER,
     .color = GREEN,
     .direction = NORTH,
     .action = IDLE,
     .x_pos = start_locations_x[guess],
     .y_pos = start_locations_y[guess],
     .health = 100,
     .damage = 50
     };
     }
     close(map_fd);
     return return_ptr;
     
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

//=========-------------
//JPL 4/13/16 This is the Method that will find which of the dev nodes that will correspond to the currently used device node
char * find_used_device()
{
     printf("Press space to continue\n");

     struct pollfd * event_files;
     char * return_file = malloc(sizeof(char)*20);
     

     struct dirent **namelist;
     
     char * file = malloc(sizeof(char)*20);
     
     int n;
     n = scandir("/dev/input", &namelist, compare_entries, alphasort);
     
     event_files = malloc(sizeof(struct pollfd)*n);
     
     for(int i =0;i< n;i++)
     {
          sprintf(file, "/dev/input/%s",namelist[i]->d_name );
          event_files[i].fd = open(file,O_RDWR);
          event_files[i].events = POLLIN;
     }
     
     int while_loop_break = 0;
     struct input_event kbdev;
     

     while(while_loop_break != -1)
     {
          for(int i = 0;i< n;i++)
          {
               if(poll(&(event_files[i]),1,5))
               {
                    read(event_files[i].fd, &kbdev,sizeof(kbdev));
                    if(kbdev.type != EV_KEY) continue;
                    if(kbdev.code == KEY_SPACE)
                         {
                         sprintf(return_file,"/dev/input/%s",namelist[i]->d_name);
                         while_loop_break = -1;
                         break;
                         }
               }
          }
     }
     
     for(int i =0;i<n;i++)
     {
          free(namelist[i]);
          close(event_files[i].fd);
     }
     
     free(namelist);
     free(event_files);
     

     /*Code example for the usage of the scandir function
      if (n < 0)
          perror("scandir");
      else {
          while (n--) {
              printf("%s\n", namelist[n]->d_name);
              free(namelist[n]);
          }
          free(namelist);
      }
      */
     
     printf("%s",return_file);
     return return_file;
}
//=========-------------
//JPL 4/15/16 This is a function that will compare the directory entry that is passed into it
int compare_entries(const struct dirent * to_compare)
{
     return (to_compare->d_name[0] == 'e') && (to_compare->d_name[1] == 'v') && (to_compare->d_name[2] == 'e');
}

//=========-------------
//JPL 4/18/16 This will check the list of entities for any of the possible movement confliction and also move the piece if it is capable of doing so
//NOTE: This will most likely be the finished method because having a different method is going to require far too much rethinking of the system
//The next game will have a more sophisticated object storage system and thusly a more better collision detection
int update_player(unsigned char direction, int index_of_player,object * items, int num_elements)
{
     int movement_check_value = 1;
     int mv_x = items[index_of_player].x_pos;
     int mv_y = items[index_of_player].y_pos;
     
     if((direction & 0x0f) > 0)
     {
     mv_y = mv_y - (direction & 0x01) + ((direction & 0x04)>>2);
     mv_x = mv_x - ((direction & 0x02)>>1) + ((direction & 0x08)>>3);
     
     for(int i =0;i< num_elements;i++)
     {
     if(items[i].health == DEAD || i == index_of_player) continue;
     movement_check_value *= (((items[i].y_pos - (mv_y))*(items[i].y_pos - (mv_y)) + (items[i].x_pos - (mv_x))*(items[i].x_pos - (mv_x))) > 2);
     if(movement_check_value == 0) break;
     }
     
     }
     
     if(movement_check_value)
     {
     if((direction & 0xf0) > 0) items[index_of_player].direction = (direction & 0xf0);
     items[index_of_player].x_pos = mv_x;
     items[index_of_player].y_pos = mv_y;
     
     }
     items[index_of_player].action = IDLE;
     
     return movement_check_value;
     
}

//=========-------------
//JPL 4/20/16 This will parse the needed options from the command line
//=========-------------
settings parse_command_line_options(int argc, char ** argv)
{
     settings inital_settings;
     //for(int i =1;i< argc;i++)
     //{
     
     //}
     initial_settings.is_host = 1;
     initial_settings.port = -1;
     initial_settings.number_of_players = 1;
     initial_settings.map_filename = "map";
     
     return intitial_settings;
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

