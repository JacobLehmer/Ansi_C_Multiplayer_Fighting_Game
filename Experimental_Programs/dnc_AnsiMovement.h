//Jacob Paul Lehmer(JPL) 3/14/16 
//This is the header file for the methods that will be used in the ansi controls test

//Functionality is as follows

//The main thread will load up a player struct that will have a normalized position based on an arbitrary cartesian coordinate system
//        There shall be a block placed at 0,0 0,10 0,-10 -10,0 and 10,0 for calibration purposes
//        The player will spawn at 3,0 
//        In this iteration there will be no combat or collision detection
//The main thread will have a list of the different objects in the game
//    The Main thread will send a pointer to an array of objects normalized to the player to the drawing thread
//The Game timer shall use the usleep method to give a desired framerate

#ifndef ansi_movementc
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/ioctl.h>


//--------------------------

//JPL 3/14/16 This struct will represent a player
#define PLAYER 0
#define BLOCK 1

#define RED 41
#define GREEN 42
#define BLUE 44

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
typedef struct
{
uint8_t type;  //0-player 1-block
uint8_t color; //41-Red 42-Green 44-Blue
uint8_t direction; //0-North 1-East 2-South 3-West
int x_pos;
int y_pos;
}object;

//JPL 3/16/16 - this is responible for buiilding the array of objects that will be passed around
//NOTE: A way needs to be found in order to create a way to build the environment, currently this is completely ad_hoc
void initialize_items(object * _items);


//--------------------------
//Control Mutexes and corresponding variables
int killall = 0;
int game_sync = -1; //This should only ever be changed by the timer method, otherwise it will be the value that is waited on


//--------------------------
//Initialization and Destruction methods

//JPL 3/14/16 This is going to diasble echo and blocking
void disable_echo(struct termios * new_t, struct termios * old_t);

//JPL 3/14/16 This will return the terminal back to original operating status
void reenable_echo(struct termios * old_t);


//--------------------------
//Each of the threads have their info stuct underneath it that needs to be passed in

//JPL 3/14/16 This is the control thread for reading user input
//TODO: do not use the stdin, use the actual device instead
void * handle_user_controls(void * _thread_info);
typedef struct
{
char command;
int checked; //same as for the graphics info
}user_controls;


//--------------------------
//Graphics
//--------------------------

//JPL 3/14/16 This is the drawing thread for the console
void * graphics_handler(void * _thread_info);
typedef struct
{
int checked; //0 for assign new 1 for new has been assigned new graphics thread waits on this value before calculating next frame in buffer, and then waits on sync before drawing
int player_index; //This ives the index for the current user's player so that the screen can be drawn to them 
int size;
object * in_game_objects;
}graphics_threadinfo;

//JPL 3/16/16 - This is the method respionsible for drawing the screen around the player
void draw_screen(graphics_threadinfo * _info,int ref_x, int ref_y, unsigned int col, unsigned int row);

//JPL 3/16/16 - This will return the ansi string corresponding to type and color with all cursor movements being relative
char * get_object(object * obj);


//JPL 3/14/16 This is the timer system for the entire game it will be using
//Pass this an uint8 corresponding to frames per second
void * game_timer(void * _ticks_per_second);

//JPL 3/16/16 This is the control c catch so that it may end properly
void control_c_catch(int catch);

//--------------------------

#endif
