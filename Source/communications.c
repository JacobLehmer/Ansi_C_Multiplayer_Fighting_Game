//Jacob Paul Lehmer(JPL) 4/20/16
//This is the communication file responsible for handling the server operations and any of the needed communications from the client to the server and vice versa

//JPL 4/20/16 This is the method responsible for the server's main loop to contain all of the needed information 
void * server_controls(void * _thread_info)
{
     settings * server_settings = (settings * ) _thread_info;

     //TODO Wait for connections from the clients here, not implemented yet but will be soon

     int number_of_game_elements = 0;

     object * items = initialize_map(&number_of_game_elements,server_settings->map_filename,server_settings->number_of_players);
     host_items = items;

     //TODO fabricate startup packets for all of the different possible connected entities
     //NEED a list of all of the different connected elements, perhaps a list of sockets?
     ////Need to put that together before i can build the system

     //temp_kludge
     startup_packet host_startup
     {
          .number_of_elements = number_of_game_elements,
          .player_index = 0,
          .other_players = server_settings->number_of_players
     }
     //end_temp_kludge
     host_startup_packet = &host_startup;
     
     while(killall != -1)
     {
     //TODO wait for clients to update their character to the server
     //Give up on waiting after the specified time tick
          for(int j=0;j< server_settings->number_of_players;j++)
          {
               switch(items[j].direction)
               {
                case NORTH:
                    {
                    for(int i =0; i< elements;i++) 
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
                    for(int i =0; i< elements;i++) 
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
                    for(int i =0; i< elements;i++) 
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
                    for(int i =0; i< elements;i++) 
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
          host_items = &items;
          //TODO need to send the updated player locations to all of the clients     
     }
}
