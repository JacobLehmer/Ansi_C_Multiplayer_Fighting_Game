//Jacob Paul Lehmer 3/18/16 
//This is the main header containing all of the defines and all of the structs 
//This also contains all of the protoypes for the utility methods
//It also contains any of the needed global variables
//--------------------------

#ifndef main_H
#define main_H
//JPL 3/14/16 This struct will represent any game entity
//object types
#define PLAYER 0
#define BLOCK 1

//object colors
#define RED 41
#define GREEN 42
#define BLUE 44

//object directions
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

//object actions
#define IDLE 0
#define ATTACKING 1
typedef struct
{
uint8_t type;  
uint8_t color; 
uint8_t direction; 
uint8_t action;
int x_pos;
int y_pos;
}object;

//--------------------------
//Control Mutexes and corresponding variables
int killall;
int game_sync;//This should only ever be changed by the timer method so a mutex is not needed, otherwise it will be the value that is waited on

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
char command;
int checked; //same as for the graphics info
}user_controls;


//--------------------------
//Utility Prototypes
//--------------------------

//JPL 3/16/16 This is the control c catch so that it may end properly
void control_c_catch(int catch);

//JPL 3/14/16 This is going to diasble echo and blocking
void disable_echo(struct termios * new_t, struct termios * old_t);

//JPL 3/14/16 This will return the terminal back to original operating status
void reenable_echo(struct termios * old_t);

//JPL 3/16/16 - this is responible for buiilding the array of objects that will be passed around
//NOTE: A way needs to be found in order to create a way to build the environment, currently this is completely ad_hoc
//possibly to read from a map file of some sort,
//or random generation
void initialize_items(object * _items);
#endif
