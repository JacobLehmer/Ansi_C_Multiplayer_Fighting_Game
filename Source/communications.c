//Jacob Paul Lehmer(JPL) 4/20/16
//This is the communication file responsible for handling the server operations and any of the needed communications from the client to the server and vice versa

#include "Headers/include_all.h"

//JPL 4/20/16 This is the method responsible for the server's main loop to contain all of the needed information 
void * server_controls(void * _thread_info)
{
     _items_not_ready = 1;
     settings * server_settings = (settings * ) _thread_info;
     
     communication_controls * communication_list = wait_for_player_connections(server_settings->number_of_players,server_settings->port);
     
     int number_of_game_elements = 0;

     object * items = initialize_map(&number_of_game_elements,server_settings->map_filename,server_settings->number_of_players);
     _items = items;
     _items_not_ready =0;
     
     send_startup_packets(communication_list,number_of_game_elements ,server_settings->number_of_players);
     send_all_data_packets(items,communication_list, number_of_game_elements,server_settings->number_of_players);
     
     
     int sync_check = server_game_sync;
     
     while(killall != -1)
     {
     //TODO Give up on waiting after the specified time tick
          wait_for_client_update(items,communication_list,server_settings->number_of_players);
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
          
          _items = items;
          _send_player_state = 0;

          for(int i =1;i< server_settings->number_of_players;i++) write(communication_list[i-1].socket, &server_game_sync, sizeof(int));
          
          sync_check = server_game_sync;

          send_player_locations(items, communication_list, server_settings->number_of_players);    
          }
          usleep(5000);
     }
     free(items);
     free(communication_list);
}

//JPL 4/22/16 This is the client control loop that is handling talking to the server
void * client_controls(void * _thread_info)
{
     _items_not_ready = 1;
     int socket = *((int *) _thread_info);
     
     recv(socket, &_startup_packet , sizeof(_startup_packet) , 0);
     
     _items = malloc(sizeof(object)*(_startup_packet.number_of_elements));
     
     for(int i =0;i< _startup_packet.number_of_elements;i++)
     {
     recv(socket, &(_items[i]), sizeof(object),0); 
     }
     _items_not_ready = 0;

     while(killall != -1)
     {
          if(_send_player_state == 1)
          {
               write(socket, &(_items[_startup_packet.player_index]), sizeof(object));
               _send_player_state = 0;
          }

          recv(socket, &(server_game_sync), sizeof(int),0); 
          
          for(int i=0;i< _startup_packet.other_players;i++)
          {
               recv(socket, &(_items[i]), sizeof(object),0);
          }     
          
          client_game_sync = server_game_sync;

     }
     close(socket);
     
                    
}
//JPL 4/22/16 This will wait until the number of specified players have connected to the system
//may need to pass in the socket_desc or do this in the server loop <- may be a better idea
communication_controls * wait_for_player_connections(int number_of_players, char * port)
{
     struct sockaddr_in server,client;
     communication_controls * connections = malloc(number_of_players*sizeof(communication_controls) - sizeof(communication_controls)); //minus one because the host does not need one
     int socket_desc;
     int c; //no clue what this does
     socket_desc = socket(AF_INET, SOCK_STREAM, 0);
     if(socket_desc < 0)
     {
          perror("Could Not Create Socket");
          killall = -1;
          return connections;
     }
     
     server.sin_family = AF_INET;
     server.sin_addr.s_addr = INADDR_ANY;
     server.sin_port = htons(atoi(port));
     
     if(bind(socket_desc,(struct sockaddr *)&server,sizeof(server)) < 0)
     {
          perror("Bind has Failed:");
          killall = -1;
          return connections;
     }
     
     listen(socket_desc,3);
     c = sizeof(struct sockaddr_in);
     
     for(int i = 1;i< number_of_players;i++)
     {
        printf("Waiting For Connections\n");
          connections[i-1].index_of_player = i;
          connections[i-1].socket = accept(socket_desc,(struct sockaddr *)&client,(socklen_t*)&c);
     }
     
     return connections;
     
}
//JPL 5/3/16 This will wait for all of the players to update to the servers
void wait_for_client_update(object * items,communication_controls * connections, int number_of_players)
{
     for(int i =1;i< number_of_players;i++) //This may need to be updated with pollfds that will allow for an asynchronous updating
     {
          recv(connections[i-1].socket, &(items[connections[i-1].index_of_player]), sizeof(object),0); 
     }
}

//JPL 4/22/16 This is the all important connect to server method, returning the socket of the connection 
int connect_to_server(char * address, char * port)
     {


     int socket_desc =0;
     socket_desc = socket(AF_INET , SOCK_STREAM , 0);
     struct sockaddr_in server;
     if (socket_desc == -1)
     {
        printf("Could not create socket");
        killall = -1;
     }
     
     server.sin_addr.s_addr = inet_addr(address);
     server.sin_family = AF_INET;
     server.sin_port = htons(atoi(port));
     
     if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
     {
        puts("connect error");
        killall = -1;
     }
     
     return socket_desc;
     }
     
//JPL 4/27/16 This will send all of the initial data packets to the players
void send_startup_packets( communication_controls * connections,int number_of_game_elements, int number_of_players)
     {
     startup_packet host_startup =
     {
          .number_of_elements = number_of_game_elements,
          .player_index = 0,
          .other_players = number_of_players
     };

     _startup_packet = host_startup;
     
     for(int i =1; i< number_of_players; i++)
        {
        startup_packet startup =
        {
          .number_of_elements = number_of_game_elements,
          .player_index = connections[i-1].index_of_player,
          .other_players = number_of_players
        };
        write(connections[i-1].socket, (&startup), sizeof(startup));
        }
     }
     
//JPL 4/27/16 This will send the entire map to all of the clients
void send_all_data_packets(object * items, communication_controls * connections,int number_of_game_elements, int number_of_players)
     {
          for(int i =0; i< number_of_game_elements;i++)
          {
               for(int j=1; j< number_of_players;j++)
               {
                 write(connections[j-1].socket, &(items[i]), sizeof(object));   
               }
          }
     }
    
//JPL 5/3/16 This will send all of the player positions
void send_player_locations(object * items, communication_controls * connections, int number_of_players)
     {
          for(int i = 1;i< number_of_players;i++)
          {
               for(int j = 0;j< number_of_players;j++)
               {
                    write(connections[i-1].socket, &(items[j]), sizeof(object));
               }
          }
     }


//JPL 4/22/16 This will recieve the startup packet from the server
startup_packet get_startup_packet(int is_host, int socket)
     {

     while(_startup_packet.number_of_elements == 0&& killall != -1)
     {
     usleep(1000);
     }
     return _startup_packet;
     
     }
     
//JPL 4/22/16 This will recieve the initial object list from the server
object * get_object_list_from_server(int is_host, int socket)
     {
     while(_items_not_ready ==1&& killall != -1)
     {
     usleep(1000);
     }
     return _items;
     }
     
//JPL 4/22/16 This will update the player locations in the current game
void update_player_locations(int is_host, int socket, object * items, startup_packet game_info)
     {
     while(client_game_sync == server_game_sync && killall != -1)
     {
     usleep(1000);
     }
     client_game_sync = server_game_sync;
     }

//JPL 4/22/16 This will send the state of the player to the server
void send_player_state(int is_host, int socket, object player)
     {
     while(_send_player_state == 1&& killall != -1)
     {
     usleep(1000);
     }
     _send_player_state = 1; //one means to send, zero means been sent
     }

