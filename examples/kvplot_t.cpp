/***************************************************************************
                           kvplot_t.cpp - v.1.10
                           ---------------------
    begin                : Thu Dec 19 2002
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

#include "viz/x11/kvplot.h"
#include <unistd.h> // sleep
#include <stdio.h>  // strings

const ve_t simple_trace[60] = 
    { 12, 13, 16, 18, 29, 32, 30, 35, 37, 39, 28, 26, 25, 22, 20, 
      18, 16, 14, 13,  9,  8,  2,  1,  7,  9, 16, 22, 30, 24, 16,
      11, 19, 13, 17, 22, 32, 30, 25, 37, 39, 32, 29, 28, 27, 24, 
      18, 16, 14, 13,  9,  8,  5,  5,  6,  7,  9, 10, 11, 11, 13};

int main(int ac, char **args) {

  WindowSettings winset;
  winset.xpos   = 1;
  winset.ypos   = 1;
  winset.width  = 250;
  winset.height = 320;
  winset.border = 2;
  winset.framerate=1;
  winset.skip   = 1;
  sprintf(winset.title, "KVPlot Demo");
  
  X11Plot *window = new X11Plot(winset);
  window->prepare_colours();
  
  KVPlot kp(window);
  
  KVector vect(80); // vector length
  Peak peak(25);   // remember the last x peaks
  
  int temp_evnt;
  int quit=false;
  ves_t psiz;
  vei_t plen;
  ve_t  pamp;
  bool  sign;
  
  printf("\nStarting KVPlot Demo, press 'q' to quit..\n\n");
  int i=0, j=0;
  while (!quit) {
    window->delay(0);
    temp_evnt = window->eventloop();
    if (temp_evnt==24)          // 'q'
        quit = true;      
    vect.add_comp(simple_trace[(i++)]+j*14); 
    if (i==60) { i=0; j++;}
    if (j==12) { j=0; }
    // show histogram in top third area:
    kp.histogram(1,4, vect,0, 58, "Histogram");
    // show scrolling plot in second third area:
    kp.timeseries(2,4, vect,2, "Time Series");
    // show peak plot in bottom third area:
    vect.get_peak(&psiz, &pamp, &plen, &sign);
    peak.add_peak( psiz,  pamp,  plen, sign);
    kp.peakplot(3,4, peak,"Peak Analysis");
    kp.textplot(4,4, vect);
    window->swap_buffers();
    window->delay(10000);
  } // while     
                  
 return 0;  
}
