/***************************************************************************
                          clustplot.h  -  v.1.00
                          ----------------------
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

#ifndef CLUSTPLOT_H
#define CLUSTPLOT_H

#include "x11plot.h"
#include "cstk_base/vector/kvector.h"

/**Clustering plotting module
  *@author Kristof Van Laerhoven
  */

class ClustPlot {
 public: 
	ClustPlot(X11Plot* pwindow=NULL);
	~ClustPlot() {};
	void barplot(int x, int y, int max_x, int max_y, 
		KVector *vector, int bgcolour=0);
	void lineplot(int x, int y, int max_x, int max_y, 
		KVector *vector, int bgcolour=0);
 private:
	X11Plot *window;
	unsigned int win_width_old, win_height_old;  
	int cell_width;
	int bar_width;
	int cell_height;
	int bar_max;
};

#endif
