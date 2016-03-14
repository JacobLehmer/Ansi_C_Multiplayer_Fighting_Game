//Jacob Paul Lehmer(JPL) 3/14/16 
//This is a working experiment to figure out the controls for the system
//See the header file for more information
#include "dnc_AnsiMovement.h"

//=========-------------
//JPL 3/14/16
int main(int argc, char ** argv)
{
struct termios new_t,old_t;
disable_echo(&new_t,&old_t);


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
//JPL 3/14/16 This is the graphics thread for the system

//=========-------------

//JPL 3/14/16 This is the timer system thread it will use usleep to time the system as it gives adequate precision for a system, 
//TODO: a calibration system loaded into the operation so that it will rebalence the amount of time that was used in order to keep stablity over long periods
void * game_timer(void * _ticks_per_second)
{
uint8_t tps = *((uint8_t *)_ticks_per_second);
unsigned int wait_time = 1000000/tps;

while(killall != 1)
     {
     sync *= -1;
     usleep(wait_time);
     }
}
