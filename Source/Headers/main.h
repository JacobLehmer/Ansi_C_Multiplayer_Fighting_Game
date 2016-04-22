//Jacob Paul Lehmer 3/18/16 
//This is the main header containing all of the defines and all of the structs 
//This also contains all of the protoypes for the utility methods
//It also contains any of the needed global variables
//--------------------------

#ifndef main_H
#define main_H

//JPL 3/14/16 This struct will represent any game entity
#define DEAD 0
#define INVINCIBLE -1

//object types
#define PLAYER 0
#define BLOCK 1

//object colors
#define RED 41
#define GREEN 42
#define BLUE 44

//object directions
#define NORTH 0x10
#define EAST 0x80
#define SOUTH 0x40
#define WEST 0x20

//object actions
#define IDLE 0
#define ATTACKING 1
typedef struct
{
uint8_t type;  
uint8_t color; 
uint8_t direction; 
uint8_t action;
int health;
uint8_t damage;
int x_pos;
int y_pos;
}object;



//--------------------------
//Control Mutexes and corresponding variables
int killall;
int server_game_sync;//This should only ever be changed by the timer method so a mutex is not needed, otherwise it will be the value that is waited on
int client_game_sync;
//--------------------------
//Thread Information Packs
//--------------------------
typedef struct
{
int checked; //0 for assign new 1 for new has been assigned new graphics thread waits on this value before calculating next frame in buffer, and then waits on sync before drawing
int player_index; //This gives the index for the current user's player so that the screen can be drawn to them 
int size;
object * in_game_objects;
}graphics_threadinfo;

typedef struct
{
char * event_filename;
unsigned char command;
unsigned char attack;
int checked; //same as for the graphics info
}user_controls;




//--------------------------
//This is an object packet for sending across the lines
//--------------------------
typedef struct
{
object _toSend;
int index;
} object_packet;


//--------------------------
//This is for starting the operations on the system
//--------------------------
typedef struct 
{
int number_of_elements;
int player_index;
int other_players;
}startup_packet;


//--------------------------
//This is the settings struct responsible for keeping the settings that are parsed initially from the command line
//Not all of these settings will be filled for each of the clients or servers
//--------------------------
typedef struct 
{
int is_host;
char * port;
int number_of_players;
int fps;
char * map_filename;
char * server_address;
}settings;


typedef struct 
{
int index_of_player;
int socket;
}communication_controls;

//--------------------------
//Utility Prototypes
//--------------------------


//JPL 3/24/16 This is a very nice copy and double method for pointer array expansion
void * copy_and_double(void * to_copy,int item_size, int size);

//JPL 3/16/16 This is the control c catch so that it may end properly
void control_c_catch(int catch);

//JPL 3/14/16 This is going to diasble echo and blocking
void disable_echo(struct termios * new_t, struct termios * old_t);

//JPL 3/14/16 This will return the terminal back to original operating status
void reenable_echo(struct termios * old_t);


//JPL 4/4/16 This will initalize all of the elements in the map file 
//pass in an int address corresponding to the size of the map
//The map file is going to be a very simple text file corresponding to a * being an invincible blue block and a @ being the origin of the map
object * initialize_map(int * size, char * filename ,int number_of_players);

//JPL 4/13/16 This is the method that will wait for input from any of the file descriptors 
//Returning the filepath of the device
char * find_used_device();

//JPL 4/15/16 This is a function that will compare the possible strings for the values seeked
int compare_entries(const struct dirent * to_compare); 

//JPL 4/18/16 This is going to be the method that will check the movement for any collisions
int update_player(unsigned char direction,int index_of_player ,object * items, int num_elements);

//JPL 4/20/16 This will parse all of the command line options in order to create the settings struct
//-h host
//-p=#### port=number 
//-u=# users=number
//-m="PATH" -map=path

//-c client
//-a="Address"
//-s=# frames per second
settings parse_command_line_options(int argc, char ** argv);

#endif
