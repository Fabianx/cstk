/***************************************************************************
                           x11plot.cpp  -  v.1.00
                           ----------------------
    begin                : Fri Nov 29 2002
    copyright            : (C) 2002-2003 by Kristof Van Laerhoven
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

#include "x11plot.h"

X11Plot::X11Plot()
{
  display = NULL;
  font_info = NULL;
  gc = NULL;
  gc_copy = NULL;
}

X11Plot::X11Plot(int x1, int x2, int y1, int y2, int bw, char* window_name)
{
  create(x1,x2,y1,y2,bw,window_name);
}

X11Plot::~X11Plot()
{
  XFreePixmap(display, buffer);
  if (font_info!=NULL) XUnloadFont(display,font_info->fid);
  if (gc!=NULL) XFreeGC(display,gc);
  if (gc_copy!=NULL) XFreeGC(display,gc_copy);
  if (display!=NULL) XCloseDisplay(display);
}

void X11Plot::create(int x1,int x2,int y1,int y2, int bw, char* window_name)
{
 char *display_name = getenv("DISPLAY");
 char *icon_name = "window";
 static XSizeHints size_hints;
   
 // open up the screen display:
  display = XOpenDisplay(display_name);
  if (display == NULL) fprintf(stderr, "Cannot connect to X server");
  
  screen_num    = DefaultScreen(display);
  screen_width  = DisplayWidth(display, screen_num);
  screen_height = DisplayHeight(display, screen_num);

 // create actual window:
  black_pixel = BlackPixel(display, screen_num);
  white_pixel = WhitePixel(display, screen_num);
  win_x = x1;
  win_y = y1;
  win_width  = x2;
  win_height = y2;
  border_width = bw;
  root_win = RootWindow(display,screen_num);
  win = XCreateSimpleWindow( display, root_win, win_x, win_y, 
                             win_width, win_height,
                             border_width, black_pixel, white_pixel );
  XSetStandardProperties(display, win, window_name, icon_name, 
                         None, 0, 0, &size_hints);
  
  ProtocolAtom = XInternAtom(display, "WM_PROTOCOLS", 0);
  KillAtom = XInternAtom(display, "WM_DELETE_WINDOW", 0);
  XSetWMProtocols(display, win, &KillAtom, 1);
  XSelectInput(display, win, ButtonPressMask | KeyPressMask | 
                             StructureNotifyMask | SubstructureNotifyMask);
  XMapWindow(display, win);

 // create the window bitmap and its copy
  int lw = 1;   // line width 
  valuemask = 0;
  gc = XCreateGC(display, win, valuemask, &values);
  if (gc<0) fprintf(stderr, "gc XCreateGC: \n");
  gc_copy = XCreateGC(display, win, valuemask, &values);
  if (gc_copy<0) fprintf(stderr, "gc_copy XCreateGC: \n");
  XSetForeground(display, gc, BlackPixel(display, screen_num));
  XSetBackground(display, gc, WhitePixel(display, screen_num));
  XSetFillStyle(display, gc, FillSolid);
  XSetLineAttributes(display, gc, lw, LineSolid, CapRound, JoinRound);
  buffer = XCreatePixmap(display,win, win_width, win_height, 
                         DefaultDepth(display, screen_num));
 // init font
  font_info = XLoadQueryFont(display, "6x10");
  if (font_info==NULL)
    font_info = XLoadQueryFont(display, "*"); // first = best
  XSetFont(display, gc, font_info->fid); 

  XSetForeground(display, gc, WhitePixel(display,screen_num));
  XFillRectangle(display, buffer, gc, 0, 0, win_width, win_height); 
  XMoveWindow(display,win,x1,y1);  
}

void X11Plot::prepare_colours()
{
  colour_map = DefaultColormap(display, screen_num);
  for (int i=0; i<16; i++) {    
    plot_colours[i].red   = plot_clrs[i][0];  
    plot_colours[i].green = plot_clrs[i][1];  
    plot_colours[i].blue  = plot_clrs[i][2];
    Status rc = XAllocColor(display, colour_map, &plot_colours[i]);
    if (rc==0) fprintf(stderr, "XAllocColor: \n");  
  }  
}

void X11Plot::draw_text(char* text_string, int x,int y, int color)
{
  XSetForeground(display, gc, plot_colours[color].pixel);
  XDrawString(display, buffer, gc, x, y, text_string, strlen(text_string) );
}

void X11Plot::swap_buffers() // for double buffering 
{
  // swap buffers 
   XSetFillStyle(display, gc_copy, FillTiled);
   XSetTile(display, gc_copy, buffer);
   XFillRectangle(display, win, gc_copy, 0, 0, win_width, win_height);
   XSync(display,win);
   XSetForeground(display, gc, WhitePixel(display,screen_num));
   XFillRectangle(display, buffer, gc, 0, 0, win_width, win_height); 
}

int X11Plot::eventloop() 
{
  XEvent xev;
  int last_key=0, num_events;
  //XFlush(display);
  num_events = XPending(display);
  while((num_events != 0))
  { 
    num_events--;
    XNextEvent(display, &xev);
    //printf("*%i\n\r",xev.type);
    switch (xev.type) 
    {
       case ConfigureNotify: 
               XFreePixmap(display, buffer);
               buffer = XCreatePixmap(display,win, xev.xconfigure.width, 
                                      xev.xconfigure.height,
                                      DefaultDepth(display, screen_num));
               win_width = xev.xconfigure.width;
               win_height = xev.xconfigure.height;
               break;
       case KeyPress:        
               last_key = xev.xkey.keycode;
               break;
       case ButtonPress:     
               last_key = xev.xbutton.button;
               break;
       case ClientMessage:
               if ((Atom)xev.xclient.message_type == ProtocolAtom
                    && (Atom)xev.xclient.data.l[0] == KillAtom )
               {
                  last_key = 24; // virtual keypress = 'q'
               }
    }
  }   
  return last_key;
}

void X11Plot::drawframe(int x, int y, int width, int height) 
{
  // draw frame in black
   XSetForeground(display, gc, BlackPixel(display,screen_num));
   XDrawRectangle(display, buffer, gc, x, y, width-1, height-1);
}

void X11Plot::drawline(int x1, int y1, int x2, int y2, int clr)
{
	XSetForeground(display, gc, plot_colours[clr].pixel);
      	XDrawLine( display, buffer, gc, x1, y1, x2, y2);
}

void X11Plot::delay(unsigned long i)
{
  usleep(i);
}
