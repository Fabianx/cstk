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

ClustPlot::ClustPlot(X11Plot* pwindow){
	win_width_old = win_height_old = 0;
	window = pwindow;
}

void ClustPlot::barplot(int x, int y, int max_x, int max_y, 
                       KVector *vector, int bgcolour)
{
	if (win_width_old != window->win_width) {
		cell_width  = (window->win_width-4)/max_x;
		bar_width   = (cell_width-2)/vector->get_dim();
		win_width_old = window->win_width;
	}
	if (win_height_old != window->win_height) {
		cell_height = (window->win_height-4)/max_y;
		bar_max     = (cell_height-2);
		win_height_old = window->win_height;
	}
	
	XSetForeground(window->display, window->gc, BlackPixel(window->display,window->screen_num));
	XDrawRectangle(window->display, window->buffer, window->gc, 2+cell_width*x, 2+cell_height*y, 
						cell_width, cell_height);
	XSetForeground(window->display, window->gc, window->plot_colours[bgcolour].pixel);
	XFillRectangle(window->display, window->buffer, window->gc, 2+cell_width*x+1, 2+cell_height*y+1, 
						cell_width-2, cell_height-2);
	
	for (vei_t i=0; i<vector->get_dim(); i++) {
		XSetForeground(window->display, window->gc, window->plot_colours[i].pixel);
		XFillRectangle(window->display, window->buffer, window->gc, 2+cell_width*x+1+(bar_width*i),
				2+cell_height*y+1+bar_max-vector->pvect[i]*bar_max/255,
				bar_width, vector->pvect[i]*bar_max/255);
	}
}

void ClustPlot::lineplot(int x, int y, int max_x, int max_y, 
                       KVector *vector, int bgcolour)
{
	if (win_width_old != window->win_width) {
		cell_width  = (window->win_width-4)/max_x;
		bar_width   = (cell_width-2)/vector->get_dim();
		win_width_old = window->win_width;
	}
	if (win_height_old != window->win_height) {
		cell_height = (window->win_height-4)/max_y;
		bar_max     = (cell_height-2);
		win_height_old = window->win_height;
	}
	
	XSetForeground(window->display, window->gc, BlackPixel(window->display,window->screen_num));
	XDrawRectangle(window->display, window->buffer, window->gc, 2+cell_width*x, 2+cell_height*y, 
						cell_width, cell_height);
	XSetForeground(window->display, window->gc, window->plot_colours[bgcolour].pixel);
	XFillRectangle(window->display, window->buffer, window->gc, 2+cell_width*x+1, 2+cell_height*y+1, 
						cell_width-2, cell_height-2);
	
	for (vei_t i=1; i<vector->get_dim(); i++) {
		XSetForeground(window->display, window->gc, window->plot_colours[i].pixel);
		XDrawLine( window->display, window->buffer, window->gc, 
			2+cell_width*x+1+(bar_width*i),
			2+cell_height*y+1+bar_max - vector->pvect[i-1]*bar_max/255,
			2+cell_width*x+1+(bar_width*i)+bar_width/2, 
			2+cell_height*y+1+bar_max - vector->pvect[i]*bar_max/255);
	}
}

