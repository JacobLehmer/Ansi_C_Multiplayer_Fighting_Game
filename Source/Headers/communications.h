//Jacob Paul Lehmer (JPL)
//This is the header responsible for the internet communication

#ifndef comms_H
#define comms_H

//These pieces of data are for the host of the server to be able to connect withut having to deal with connecting to itself
object * host_items;
//int check_update;

startup_packet * host_startup_packet;

//JPL 4/18/16 This is the main server loop
//This will be repsonsible for handling collision gemotery and relaying the timesync 
void * server_controls(void * _thread_info);

//JPL 4/20/16 This is the client loop
void * client_controls(void * _thread_info);

//JPL 4/22/16 This will create the server and wait until the specified number of players have connected and then
//return the array of connections
communication_controls * wait_for_player_connections(int number_of_players, int port);

//JPL 4/18/16 This will initialize the server
int initailize_server(int port);

//JPL 4/18/16 This will send the initial data packet to all of the clients
void send_data_packet(object * items, communication_controls * connections, int number_of_players);

//JPL 4/18/16 This will send the list of objects in the game
void send_all_data_packets(object * items, communication_controls * connections, int number_of_players);

//JPL 4/18/16 This will send all of the updated player locations
void send_player_locations(object * items, communication_controls * connections, int number_of_players);

//JPL 4/22/16 This will wait for all of the clients to send their updated locations to the server
void wait_for_client_update(object * items, communication_controls * connections, int sync_check);

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

#endif
