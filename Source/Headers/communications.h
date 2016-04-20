//Jacob Paul Lehmer (JPL)
//This is the header responsible for the internet communication

//These pieces of data are for the host of the server to be able to connect withut having to deal with connecting to itself
object * host_items;
int check_update = 0;

startup_packet * host_startup_packet;

//JPL 4/18/16 This is the main server loop
//This will be repsonsible for handling collision gemotery and relaying the timesync 
void * server_controls(void * _thread_info);

//JPL 4/20/16 This is the client loop
void * client_controls(void * _thread_info);

//JPL 4/18/16 This will initialize the server
void initailize_server(int port, char * map, int players);

//JPL 4/18/16 This will send the initial data packet
void send_data_packet();

//JPL 4/18/16 This will send the list of objects in the game
void send_all_data_packets();

//JPL 4/18/16 This will send all of the updated player locations
void send_player_locations();

//JPL 4/20/16 This will connect to the server returning the connected socket on sucess
int connect_to_server(char * address, int port);

//JPL 4/20/16 This will recieve the initial data packet
startup_packet get_startup_packet(int is_host, int socket);

//JPL 4/20/16 This will recieve and create the list of objects in the game
object * get_object_list_from_server(int is_host, int socket);

//JPL 4/20/16 This will recieve the updated player locations
void update_player_locations(int is_host, int socket, object * items, startup_packet game_info);

//JPL 4/20/16 This will send the player state to the server
void send_player_state(int is_host, int socket, object player);
