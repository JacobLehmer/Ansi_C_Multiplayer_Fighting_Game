//Jacob Paul Lehmer (JPL)
//This is the header responsible for the internet communication

//JPl 4/18/16 This is the main server loop
void * server_controls(void * _thread_info);

//JPL 4/20/16 This is the client loop
void * client_controls(void * _thread_info);

//JPL 4/18/16 This will initialize the server
void initailize_server(int port, char * map, int players);

//JPL 4/18/16 This will send the initial data packet

//JPL 4/18/16 This will send the list of objects in the game

//JPL 4/18/16 This will send all of the updated player locations

//JPL 4/20/16 This will recieve the initial data packet

//JPL 4/20/16 This will recieve the list of objects in the game

//JPL 4/20/16 This will recieve the updated player locations

//JPL 4/20/16 This will send the player state to the server
