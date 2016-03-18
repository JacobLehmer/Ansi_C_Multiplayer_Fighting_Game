//Jacob Paul Lehmer (JPL) /3/18/16
//This is the file for the syncronization of the game

#include "Headers/include_all.h"
//JPL 3/14/16 This is the timer system thread it will use usleep to time the system as it gives adequate precision for a system, 
//TODO: a calibration system loaded into the operation so that it will rebalence the amount of time that was used in order to keep stablity over long periods
void * game_timer(void * _ticks_per_second)
{
uint8_t tps = *((uint8_t *)_ticks_per_second);
unsigned int wait_time = 1000000/tps;

while(killall != 1)
     {
     game_sync *= -1;
     usleep(wait_time);
     }
}
