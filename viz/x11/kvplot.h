/***************************************************************************
                          kvplot.h  -  v.1.00
                          -------------------
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

#ifndef KVPLOT_H
#define KVPLOT_H

#include "x11plot.h"
#include "cstk_base/vector/kvector.h"
#include "algorithms/peak/peak.h"

/**KVector plotting module
  *@author Kristof Van Laerhoven
  */

struct WindowSettings {
	WindowSettings() {
		xpos = ypos = width= height=-1;
		border=framerate=skip = -1;
		title[0]='\0';
   }
   int xpos,ypos,width,height,border,framerate,skip;
   char title[256];
};

class KVPlot : public X11Plot {
 public: 
  KVPlot(WindowSettings ws);
  char histogram(uint cscr, uint tscr, KVector& vector, int colour, 
                 uint num_buckets, char* title=NULL); 
  char timeseries(uint cscr, uint tscr, KVector& vector, int colour, 
                  char* title=NULL, bool scaling=1);  
  char peakplot(uint cscr, uint tscr, Peak& peak, char* title=NULL); 
  
  char textplot(uint cscr, uint tscr, KVector& vector);

  char spiketrain(uint cscr, uint tscr, KVector& vector, int colour, 
                  char* title=NULL);    
  char impulse(uint cscr, uint tscr, KVector& vector, int colour, 
               char* title=NULL);  
  
 private: 
  XPoint plot_point[1024];
  // variables for the plots that do not change very often:
  unsigned int sub_height;
  unsigned int tot_height;
  unsigned int bucket_width;
  vei_t        peak_width;
    
  unsigned int old_win_height;
  unsigned int old_win_width;
};

#endif
