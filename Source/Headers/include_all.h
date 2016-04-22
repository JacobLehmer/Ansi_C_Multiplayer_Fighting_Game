//Jacob Paul Lehmer (JPL) 3/18/16 
//This is the unified include file so that everything else can simply include it

#ifndef incld_all_h
#define incld_all_h
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <poll.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "main.h"
#include "communications.h"
#include "game_sync.h"
#include "graphics.h"
#include "user_input.h"
#endif
