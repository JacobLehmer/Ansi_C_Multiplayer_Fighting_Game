//Jacob Paul Lehmer (JPL) 3/18/16
//This is the main file for the operation fo the ansi fighting game

#include "Headers/include_all.h"

//JPL 3/14/16 - The main method, in addition to setting up the system will also be acting as the game engine
int main(int argc, char ** argv)
{

srand(time(NULL));
killall =0;
server_game_sync = -1;
client_game_sync = -1;


struct termios new_t,old_t;
disable_echo(&new_t,&old_t);
     
     
     settings game_settings = parse_command_line_options(argc,argv);
     

     pthread_t communication_thread;     

     pthread_t graphics_thread;
     pthread_t ui_thread;
     
     if(game_settings.is_host == 1)
     {
          pthread_t timer_thread;
          uint8_t tps = game_settings.fps;
          
          if(pthread_create(&communication_thread,NULL, server_controls,(void*)&game_settings) < 0)
          {
          perror("Server Thread Creation Failure");
          return 1;
          }
          

          if(pthread_create(&timer_thread,NULL,game_timer,(void*)&tps ) <0)
          {
          perror("Timer Thread Creation Failure");
          return 1;
          }
     }
          
     
     user_controls info_controls =
     {
     .checked = 1,
     .command = ' ',
     .event_filename = find_used_device()
     };
     
     signal(SIGINT,control_c_catch);
     
     
     if(pthread_create(&ui_thread,NULL,handle_user_controls,(void*)&info_controls) < 0)
     {
     perror("User Control Thread Creation Failure");
     return 1;
     }
     
     
     int socket = -1;
     if(game_settings.is_host == 0)
     {
          socket = connect_to_server(game_settings.server_address, game_settings.port);
          if(socket == -1)
          {
               perror("Connection Error");
               return 1;
          }
          if(pthread_create(&communication_thread,NULL, client_controls,(void*)&socket) < 0)
          {
          perror("Client Thread Creation Failure");
          return 1;
          }
           
     }
     
     startup_packet game_info = get_startup_packet(game_settings.is_host, socket);
     object * items = get_object_list_from_server(game_settings.is_host, socket);
     
    
     
     graphics_threadinfo info_graphics =
     {
     .checked = 1,
     .player_index = game_info.player_index,
     .size = game_info.number_of_elements,
     .in_game_objects = items
     };
     
     if(pthread_create(&graphics_thread,NULL,graphics_handler,(void*)&info_graphics) < 0)
     {
     perror("Graphics Thread Creation Failure");
     return 1;
     }

     int timer_sync = client_game_sync;
     int attack_timer = 0;
     int just_hit = 0;
     int movement_check = 1;
     
     int elements = game_info.number_of_elements;

     int player_index = game_info.player_index;
     
     
     while(killall != -1)
     {

          if(timer_sync != client_game_sync && items[player_index].action == ATTACKING)
          {
          attack_timer ++;
          timer_sync = client_game_sync;
          }
     
          if(info_controls.checked != 1)
          {
               update_player(info_controls.command,player_index, items,elements);  
               info_graphics.checked = 1;   
               info_controls.checked = 1;
          }
          
          if(info_controls.attack ==0x01)
               {
               items[player_index].action = ATTACKING;
               info_graphics.checked = 1;
               }          
          
          if(attack_timer >=2)
          {
               items[player_index].action = IDLE;
               attack_timer = 0;
               just_hit = 0;
               info_graphics.checked = 1;
          }
          printf("::-::%d,%d\n",player_index,items[player_index].direction);
          send_player_state(game_settings.is_host,socket,items[player_index]);
          update_player_locations(game_settings.is_host,socket,items,game_info);
          
          
          if(attack_timer ==1 && just_hit == 0)
          {
          just_hit = 1;
          }
          
          
     usleep(1000);
     }
     
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
     settings initial_settings;
     initial_settings.is_host = 1;
     initial_settings.number_of_players = 1;
     initial_settings.map_filename = "map";
     initial_settings.fps = 10;
     
     for(int i =1;i< argc;i++)
     {
          for(int j=0; j< 2;j++)
          {
               if(j == 0)
               {
                    if(argv[i][j] != '-')
                         {
                         perror("Command Line Format Failure: Dash Needed");
                         }
               }
          switch(argv[i][j])
               {
               case 'h': initial_settings.is_host = 1;break;
               case 'c': initial_settings.is_host = 0;break;
               case 'p': initial_settings.port = (argv[i]+j+2);break;
               case 'u': initial_settings.number_of_players = atoi((argv[i]+j+2));break;
               case 's': initial_settings.fps = atoi((argv[i]+j+2));break;
               case 'a': initial_settings.server_address = (argv[i]+j+2);break;
               case 'm': initial_settings.map_filename = (argv[i]+j+2);break;
               }
          }
     }

     return initial_settings;
}

