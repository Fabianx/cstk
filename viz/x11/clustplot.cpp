/***************************************************************************
                           clustplot.cpp  -  v.1.00
                           ------------------------
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

#include "clustplot.h"

ClustPlot::ClustPlot(){
}

ClustPlot::~ClustPlot(){
}

void ClustPlot::barplot(uint x, uint y, uint max_x, uint max_y, 
                       KVector *vector, int bgcolour)
{
  int cell_width  = (win_width-4)/max_x;
  int cell_height = (win_height-4)/max_y;
  int bar_width   = (cell_width-2)/vector->get_dim();
  int bar_max     = (cell_height-2);
  
  XSetForeground(display, gc, BlackPixel(display,screen_num));
  XDrawRectangle(display, buffer, gc, 2+cell_width*x, 2+cell_height*y, 
                                               cell_width, cell_height);
  XSetForeground(display, gc, plot_colours[bgcolour].pixel);
  XFillRectangle(display, buffer, gc, 2+cell_width*x+1, 2+cell_height*y+1, 
                                              cell_width-2, cell_height-2);
  
  for (vei_t i=0; i<vector->get_dim(); i++) {
       XSetForeground(display, gc, plot_colours[i].pixel);
       XFillRectangle(display, buffer, gc, 2+cell_width*x+1+(bar_width*i),
                      2+cell_height*y+1+bar_max-vector->pvect[i]*bar_max/255,
                      bar_width, vector->pvect[i]*bar_max/255);
  }
  
}

