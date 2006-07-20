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

class KVPlot {
 public: 
	KVPlot(X11Plot* pwindow=NULL);
	char histogram(uint cscr, uint tscr, KVector& vector, int colour, 
			uint num_buckets, char* title=NULL); 
	char timeseries(uint cscr, uint tscr, KVector& vector, int colour, 
			char* title=NULL, bool scaling=1);  
	char peakplot(uint cscr, uint tscr, Peak& peak, char* title=NULL); 
	
	char textplot(uint cscr, uint tscr, KVector& vector);

	char labelplot(uint cscr, uint tscr, char* title, uint line=0, uint col=0, uint font=0);
	
	char spiketrain(uint cscr, uint tscr, KVector& vector, int colour, 
			char* title=NULL);    
	char impulse(uint cscr, uint tscr, KVector& vector, int colour, 
		char* title=NULL);  
  
 private: 
	X11Plot *window;
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
