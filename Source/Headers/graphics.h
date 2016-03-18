//Jacob Paul Lehmer (JPL) 3/18/16 
//This is the graphics header implementing everything that is needed to draw the objects here
#ifndef graphics_H
#define graphics_H

//JPL 3/14/16 This is the drawing thread for the console
void * graphics_handler(void * _thread_info);

//JPL 3/16/16 - This is the method respionsible for drawing the screen around the player
void draw_screen(graphics_threadinfo * _info,int ref_x, int ref_y, unsigned int col, unsigned int row);

//JPL 3/16/16 - This will return the ansi string corresponding to type and color with all cursor movements being relative
char * get_object(object * obj);



#endif
