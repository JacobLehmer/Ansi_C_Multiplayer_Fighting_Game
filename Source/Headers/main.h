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
int health;
uint8_t damage;
int x_pos;
int y_pos;
}object;



//--------------------------
//This is the collision detection struct
//This will be a two dimensional array containing all of the objects based on the first element in each row being an x value
//The following elements after the first row will be the y values corresponding to the x values this will be a form of grid 
//that will use two binary searches in order to find the element that is being seeked
/*
typedef struct
{
int * number_of_rows; //interestingly, this is not an array
x_case * in_game_objects; //Two dimensional array
}collision_detection_storage;

typedef struct
{
int y;
object * item;
}y_case;

typedef struct
{
int x;
int number_of_ys;
y_case * ys;
}x_case; 
*/


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
char * event_filename;
unsigned char command;
unsigned char attack;
int checked; //same as for the graphics info
}user_controls;



//--------------------------
//Utility Prototypes
//--------------------------
//JPL 3/18/16 This is the method that will build the collision detection storage object
//colision_detection_storage * build_collision_detection_storage(object * items, int size);

//JPL 3/24/16 This is a very nice binary search returning the index of an item requesting an item
//returns standard -1 if not found
int binary_search(int * array, int array_size, int item);

//JPL 3/24/16 This is a very nice binary search returning the index of an item requesting an item starting at a given initial and final index
//returns standard -1 if not found
int binary_search_init_pos(int * array, int start, int end, int item);

//JPL 3/24/16 This is a adjacency binary search that will return a position that the current location is bigger and the previous location is smaller than the item
int adjacency_binary_search(int * array, int start, int end, int item);

//JPL 3/24/16 This is a binary search to traverse the game map 
//int binary_search_map(x_case * game_map, int size_of_x, int value);

//JPL 3/24/16 This is a very nice copy and double method for pointer array expansion
void * copy_and_double(void * to_copy,int item_size, int size);

//JPL 3/24/16 This is an insert in order method for inserting an element in order in an array may need to be changed into a more sophisticated data structure to work better
//this will expand the array if it is too small
void insert_in_order(int * array,int current_array_size, int max_array_size, int value);

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

//JPL 4/4/16 This will initalize all of the elements in the map file 
//pass in an int address corresponding to the size of the map
//The map file is going to be a very simple text file corresponding to a * being an invincible blue block and a @ being the origin of the map
object * initialize_map(int * size, char * filename);

//JPL 4/13/16 This is the method that will wait for input from any of the file descriptors 
//Returning the filepath of the device
char * find_used_device();

//JPL 4/15/16 This is a function that will compare the possible strings for the values seeked
int compare_entries(const struct dirent * to_compare); 


#endif
