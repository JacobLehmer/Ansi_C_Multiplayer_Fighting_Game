//Jacob Paul Lehmer (JPL) 3/18/16 
//This header is for controlling the input from the user

#ifndef user_H
#define user_H

//JPL 3/14/16 This is the control thread for reading user input
//TODO: do not use the stdin, use the actual device instead
//NOTE: as of 3/18/16 This is reading stdin and using the character values to obtain functionality
void * handle_user_controls(void * _thread_info);

#endif
