/***************************************************************************
                          x11plot.h  -  plot sensordata
                          -----------------------------
    begin                : Fri Nov 29 2002
    copyright            : (C) 2002 by Kristof Van Laerhoven
    email                :  
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef X11PLOT_H
#define X11PLOT_H

#include <X11/Xlib.h> // xlib
#include <X11/Xutil.h> // xlib
#include <sys/time.h> // xlib
#include <sys/select.h> // select(), ...
#include <stdlib.h> // getenv(), ..
#include <stdio.h> // stderr(),..
#include <string.h> // strlen() ,..
#include <unistd.h> // usleep() in delay function

struct WindowSettings {
	WindowSettings() {
		xpos = ypos = width= height=-1;
		border=framerate=skip = -1;
		title[0]='\0';
   }
   int xpos,ypos,width,height,border,framerate,skip;
   char title[256];
};

const int plot_clrs[16][3] = { 
  {0xFFFF, 0x0000, 0x0000}, {0x0000, 0xBFFF, 0x0000},  // red, green
  {0x0000, 0x0000, 0xFFFF}, {0xFFFF, 0xAFFF, 0x1000},  // blue, dyellow
  {0xBFFF, 0x0000, 0xFFFF}, {0x0000, 0xFFFF, 0xFFFF},  // magenta, cyan
  {0xFFFF, 0x5FFF, 0x0000}, {0x5FFF, 0x0FFF, 0x0FFF},  // orange, brown
  {0x7FFF, 0x7FFF, 0x7FFF}, {0xAFFF, 0x0000, 0x0000},  // grey, lgreen
  {0x0000, 0xAFFF, 0x0000}, {0x0000, 0x0000, 0xAFFF},  // lred, lblue, 
  {0xFFFF, 0xCFFF, 0x0000}, {0xFFFF, 0x4000, 0xBFFF},  // yellow, dmagenta
  {0x0000, 0x0000, 0x0000}, {0xFFFF, 0xFFFF, 0xFFFF} };// black, white

/**Main x11 plotting module
  *@author Kristof Van Laerhoven
  */

class X11Plot 
{
 public: 
  X11Plot();
  X11Plot(WindowSettings ws);
  X11Plot(int x1, int x2, int y1, int y2, int bw, char* window_name=0);
  ~X11Plot();
  void create(int x1, int x2, int y1, int y2, int bw, char* window_name=0);
  void prepare_colours(); // prepare the display's colours 
  void swap_buffers();    // double buffering
  void draw_text(char* text_string, int x, int y, int color);
  int eventloop();
  void drawframe(int x, int y, int width, int height); 
  void drawline(int x1, int y1, int x2, int y2, int clr);
  void delay(unsigned long i);   
  Display* display;
  unsigned int screen_width;
  unsigned int screen_height;
  Window win;
  Window root_win;
  unsigned long white_pixel;
  unsigned long black_pixel;
  unsigned int win_x, win_y;
  unsigned long bg_colour;
  unsigned int border_width;
  unsigned long border_colour;
  unsigned int win_width;
  unsigned int win_height;
  GC gc;                   // graphic context
  GC gc_copy;              // gc for double buffering
  XGCValues values;        // attribute values for the graphics context
  unsigned long valuemask; // checked values in graphics context 
  Colormap colour_map;
  XColor plot_colours[16]; // mapped colours for plotting 
  unsigned int screen_num; // id
  Pixmap buffer;           // double buffer
 private: 
  XFontStruct* font_info;
  Atom ProtocolAtom, KillAtom; // for destruction
};

#endif
