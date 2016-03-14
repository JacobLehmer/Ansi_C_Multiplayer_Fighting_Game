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
#include <termios.h>
#include <unistd.h>
#include <pthread.h>


//--------------------------

//JPL 3/14/16 This struct will represent a player
#define PLAYER 0
#define BLOCK 1

#define RED 0
#define GREEN 1
#define BLUE 2
typedef struct
{
uint8_t type;  //0-player 1-block
uint8_t color; //0-Red 1-Green 2-Blue
int x_pos;
int y_pos;
}object;


//--------------------------
//Control Mutexes and corresponding variables
int killall = 0;
int sync = -1; //This should only ever be changed by the timer method, otherwise it will be the value that is waited on


//--------------------------
//Initialization and Destruction methods

//JPL 3/14/16 This is going to diasble echo and blocking
void disable_echo(struct termios * new_t, struct termios * old_t);

//JPL 3/14/16 This will return the terminal back to original operating status
void reenable_echo(struct termios * old_t);


//--------------------------
//Each of the threads have their info stuct underneath it that needs to be passed in

//JPL 3/14/16 This is the control thread for reading user input
void * handle_user_controls(void * _thread_info);

//JPL 3/14/16 This is the drawing thread for the console
void * graphics_handler(void * _thread_info);
typedef struct
{
int checked; //0 for assign new 1 for new has been assigned new graphics thread waits on this value before calculating next frame in buffer, and then waits on sync before drawing 
object * in_game_objects;
}graphics_threadinfo

//JPL 3/14/16 This is the timer system for the entire game it will be using
//Pass this an uint8 corresponding to frames per second
void * game_timer(void * _ticks_per_second);



//--------------------------

#endif
