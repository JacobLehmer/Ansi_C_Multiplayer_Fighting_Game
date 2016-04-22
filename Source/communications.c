//Jacob Paul Lehmer(JPL) 4/20/16
//This is the communication file responsible for handling the server operations and any of the needed communications from the client to the server and vice versa

#include "Headers/include_all.h"

//JPL 4/20/16 This is the method responsible for the server's main loop to contain all of the needed information 
void * server_controls(void * _thread_info)
{
     settings * server_settings = (settings * ) _thread_info;
     
     //TODO Need to initialize the server here
     initialize_server(server_settings->port);
     //TODO Wait for connections from the clients here, not implemented yet but will be soon

     int number_of_game_elements = 0;

     object * items = initialize_map(&number_of_game_elements,server_settings->map_filename,server_settings->number_of_players);
     host_items = items;

     //TODO fabricate startup packets for all of the different possible connected entities
     //NEED a list of all of the different connected elements, perhaps a list of sockets?
     ////Need to put that together before i can build the system

     //temp_kludge
     startup_packet host_startup =
     {
          .number_of_elements = number_of_game_elements,
          .player_index = 0,
          .other_players = server_settings->number_of_players
     };
     //end_temp_kludge
     host_startup_packet = &host_startup;
     int sync_check = server_game_sync;
     
     while(killall != -1)
     {
     //TODO wait for clients to update their character to the server
     //Give up on waiting after the specified time tick
     //wait_for_client_update();
          if(sync_check != server_game_sync/*or all clients have updated*/)
          {
               for(int j=0;j< server_settings->number_of_players;j++)
               {
                    if(items[j].action == ATTACKING)
                    {
                         switch(items[j].direction)
                         {
                          case NORTH:
                              {
                              for(int i =0; i< number_of_game_elements;i++) 
                                   {
                                   if((i==j)|| items[i].health <= DEAD) continue;
                                   if(((items[i].y_pos - (items[j].y_pos-2))*(items[i].y_pos - (items[j].y_pos-2)) + (items[i].x_pos - items[j].x_pos)*(items[i].x_pos - items[j].x_pos)) <= 2)
                                        {
                                        if(items[i].health < items[j].damage) items[i].health =DEAD;
                                        else items[i].health -= items[j].damage; 
                                        }
                                   }
                              };break; 
                         case EAST:
                              {
                              for(int i =0; i< number_of_game_elements;i++) 
                                   {
                                   if((i==j)||items[i].health <= DEAD) continue;
                                   if(((items[i].y_pos - (items[j].y_pos))*(items[i].y_pos - (items[j].y_pos)) + (items[i].x_pos - (items[j].x_pos+2))*(items[i].x_pos - (items[j].x_pos+2))) <= 2)
                                        {
                                        if(items[i].health < items[j].damage) items[i].health =DEAD;
                                        else items[i].health -= items[j].damage; 
                                        }
                                   }
                              };break; 
                         
                         case SOUTH:
                              {
                              for(int i =0; i< number_of_game_elements;i++) 
                                   {
                                   if((i==j)||items[i].health <= DEAD) continue;
                                   if(((items[i].y_pos - (items[j].y_pos+2))*(items[i].y_pos - (items[j].y_pos+2)) + (items[i].x_pos - items[j].x_pos)*(items[i].x_pos - items[j].x_pos)) <=2)
                                        {
                                        if(items[i].health < items[j].damage) items[i].health =DEAD;
                                        else items[i].health -= items[j].damage; 
                                        }
                                   }
                              };break;
                         case WEST:
                              {
                              for(int i =0; i< number_of_game_elements;i++) 
                                   {
                                   if((i==j)||items[i].health <= DEAD) continue;
                                   if(((items[i].y_pos - (items[j].y_pos))*(items[i].y_pos - (items[j].y_pos)) + (items[i].x_pos - (items[j].x_pos-2))*(items[i].x_pos - (items[j].x_pos-2))) <= 2)
                                        {
                                        if(items[i].health < items[j].damage) items[i].health =DEAD;
                                        else items[i].health -= items[j].damage; 
                                        }
                                   }
                              };break;
                              
                         }
                    }
               }
          sync_check = server_game_sync;
          host_items = items;
          //TODO need to send the updated player locations to all of the clients
          //send_payer_locations();    
          }
          usleep(5000);
     }
     free(items);
}

//JPL 4/22/16 This is the client control loop that is handling talking to the server
void * client_controls(void * _thread_info)
{
     int socket = *((int *) _thread_info);
     
     
                    
}
//JPL 4/22/16 This will wait until the number of specified players have connected to the system
//may need to pass in the socket_desc or do this in the server loop <- may be a better idea
communication_controls * wait_for_player_connections(int number_of_players, char * port)
{
     struct sockaddr_in, client;
     communication_controls * connections = malloc(number_of_players*sizeof(communication_controls) - 1); //minus one because the host does not need one
     int socket_desc;
     int c; //no clue what this does
     socket_desc = socket(AF_INET, SOCK_STREAM, 0);
     if(socket_desc < 0)
     {
          perror("Could Not Create Socket");
          killall = -1;
          return 1;
     }
     
     server.sin_family = AF_INET;
     server.sin_addr.s_addr = INADDR_ANY;
     server.sin_port = htons(port);
     
     if(bind(socket_desc,(struct sockaddr *)&server,sizeof(server)) < 0)
     {
          perror("Bind has Failed:");
          killall = -1;
          return 1;
     }
     
     listen(socket_desc,3);
     c = sizeof(struct sockaddr_in);
     
     for(int i = 1;i< number_of_players;i++)
     {
          connections[i-1]index_of_player = i;
          connections[i-1]socket = accept(socket_desc,(struct sockaddr *)&client,(socklen_t*)&c);
     }
     //TODO make a loop that will accept incoming connections and assign them to a communication control
     
}

//JPL 4/22/16 This is the all important connect to server method, returning the socket of the connection 
int connect_to_server(char * address, int port)
     {
     return 1;
     }


//JPL 4/22/16 This will recieve the startup packet from the server
startup_packet get_startup_packet(int is_host, int socket)
     {
     if(is_host == 1)
          {
          while(host_startup_packet == NULL)
          {
          usleep(100);
          }
          return *host_startup_packet;
          }
     }
     
//JPL 4/22/16 This will recieve the initial object list from the server
object * get_object_list_from_server(int is_host, int socket)
     {
     if(is_host ==1)
          {
          return host_items;
          }
     }
     
//JPL 4/22/16 This will update the player locations in the current game
void update_player_locations(int is_host, int socket, object * items, startup_packet game_info)
     {
     if(is_host ==1)
          {
          while(client_game_sync == server_game_sync)
          {
          usleep(1000);
          }
          client_game_sync = server_game_sync;
          return;
          }
     }

//JPL 4/22/16 This will send the state of the player to the server
void send_player_state(int is_host, int socket, object player)
     {
     if(is_host ==1)
          {
          return;
          }
     }

