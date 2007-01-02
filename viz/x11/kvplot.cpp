/***************************************************************************
                            kvplot.cpp  -  v.1.00
                            ---------------------
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

#include "kvplot.h"

KVPlot::KVPlot(X11Plot* pwindow)
{
	old_win_height = old_win_width = 0;
	window = pwindow;
}

char KVPlot::histogram(uint cscr, uint tscr, KVector& vector, int colour, 
                        uint num_buckets, char* title) 
{
    window->drawframe(0, ((cscr-1)*window->win_height)/tscr,
                      window->win_width, window->win_height/tscr);
    // read each value for bar and draw it
    if (window->win_height!=old_win_height) {
		sub_height= (window->win_height/tscr-14);
		old_win_height = window->win_height;
    }
    bucket_width = (window->win_width-4)/num_buckets;
    uint bar = 0, max = 0;
    for (uint i=0;i<num_buckets;i++) {
		// get the fullest bucket i
		bar = vector.get_histo(num_buckets, i);
		if (max<bar)  max = bar;
    }
    XSetForeground(window->display, window->gc,
                       window->plot_colours[colour].pixel);

    for (uint i=0;i<num_buckets;i++) {
        // get the value of bucket i
        bar = vector.get_histo(num_buckets, i);
        // plot current bar
        if (max == 0) max = 1; // Stop those lovely exceptions
        XFillRectangle(window->display, window->buffer, window->gc,
                       2+i*(window->win_width-4)/num_buckets,
                       ((cscr*window->win_height)/tscr-3)-(bar*sub_height/max),
                       bucket_width, (bar*sub_height/max));
    }
    if (title!=NULL)
        window->draw_text(title, 3,((cscr-1)*window->win_height)/tscr+9, 2);
    return 0;
}

char KVPlot::timeseries(uint cscr, uint tscr, KVector& vector, int colour, 
                         char* title, bool scaling) 
{
    char tmpstr[15];

    window->drawframe(0, ((cscr-1)*window->win_height)/tscr,
                          window->win_width, window->win_height/tscr);
    if (window->win_height!=old_win_height) {
		sub_height= (window->win_height/tscr-14);
		old_win_height = window->win_height;
    }
    tot_height=(int)((cscr*window->win_height)/tscr-3);

    int scaling_fact=20;

    for (vei_t i=0; i<vector.pvect_size; i++) {
		plot_point[i].x = 2+i*(window->win_width-14)/vector.pvect_size;
		if (!scaling)
			plot_point[i].y = tot_height-(vector.pvect[i]*sub_height)/255;
		else {
			if (vector.max()-vector.min()>20) 
				scaling_fact = vector.max()-vector.min();
			plot_point[i].y = 
				(short int) (tot_height-
				((vector.pvect[i]-vector.min())*sub_height)
                                             /(0.1+scaling_fact));
		}
    }
    
	// plot the line():
	XSetForeground(window->display, window->gc, window->plot_colours[colour].pixel);
	XDrawLines(window->display, window->buffer, window->gc, plot_point, 
		vector.pvect_size, CoordModeOrigin);	
	// plot the stats:
	window->drawline(window->win_width-12,
		tot_height-(vector.max()*sub_height)/255, window->win_width-3,
		tot_height-(vector.max()*sub_height)/255, colour);
        if (title!=NULL)
        {
            sprintf( tmpstr, "max:%i", vector.max());
            window->draw_text(tmpstr, window->win_width-50,
                              tot_height-(vector.max()*sub_height)/255, 0);
        }
        window->drawline(window->win_width-12, 
		tot_height-(vector.min()*sub_height)/255,
		window->win_width-3,
                         tot_height-(vector.min()*sub_height)/255, colour);
        if (title!=NULL)
        {
            sprintf( tmpstr, "min:%i", vector.min());
            window->draw_text(tmpstr, window->win_width-50,
                              tot_height-(vector.min()*sub_height)/255, 1);
	}
        window->drawline(window->win_width-12,
		tot_height-(vector.mean()*sub_height)/255,
		window->win_width-3,
                         tot_height-(vector.mean()*sub_height)/255, colour);
        if (title!=NULL)
        {
            sprintf( tmpstr, "avg:%i", vector.mean());
            window->draw_text(tmpstr, window->win_width-50,
                              tot_height-(vector.mean()*sub_height)/255, 3);
	}
        // plot title:
	if (title!=NULL) {
		window->draw_text(title, 3,
                                  ((cscr-1)*window->win_height)/tscr+9, 2);
	}
	return 0;   
}
  
char KVPlot::peakplot(uint cscr, uint tscr, Peak& peak, char* title) 
{ 
	window->drawframe(0, ((cscr-1)*window->win_height)/tscr,
                          window->win_width, window->win_height/tscr);
	// read each value for bar and draw it
	ves_t curpeak = 0;
	ve_t  curamp = 0;
	ves_t max = 0;
	ve_t  maxamp = 0;
	if (window->win_height!=old_win_height) {
	sub_height = (window->win_height/tscr-14);
	old_win_height = window->win_height;
	} 
	peak_width = (window->win_width-4)/peak.ppeak_size;
	for (pei_t i=0;i<peak.ppeak_size;i++) {
		// get the fullest bucket i
		curpeak = abs(peak.ppeak_s[i]);
		curamp  = abs(peak.ppeak_a[i]);
		if (max<curpeak)  max = curpeak;
		if (maxamp<curamp) maxamp = curamp;
	}
	if (max==0) max = 1;
	if (maxamp==0) maxamp = 1;
	for (pei_t i=0;i<peak.ppeak_size;i++) {
		// get the value of bucket i
		curamp = peak.ppeak_a[i];
		curpeak = ((peak.ppeak_p[i]==1)?1:-1)*peak.ppeak_s[i];
		// plot current bar
		if (curpeak<0) {
			XSetForeground(window->display, window->gc, window->plot_colours[2].pixel);
			XFillRectangle(window->display, window->buffer, window->gc, 
					2+i*(window->win_width-4)/peak.ppeak_size,
					((cscr*window->win_height)/tscr-3)-(window->win_height/(2*tscr)),
					peak_width, (abs(curpeak)*sub_height/max)/2);
			window->drawline(2+i*(window->win_width-4)/peak.ppeak_size+peak_width/2,
					((cscr*window->win_height)/tscr-3)-(window->win_height/(2*tscr)),
					2+i*(window->win_width-4)/peak.ppeak_size+peak_width/2,
					((cscr*window->win_height)/tscr-3)-(window->win_height/(2*tscr))
					+(curamp*sub_height/maxamp)/2,14);              
		}
		else {
			XSetForeground(window->display, window->gc, window->plot_colours[1].pixel);
			XFillRectangle(window->display, window->buffer, window->gc, 
					2+i*(window->win_width-4)/peak.ppeak_size,
					(((cscr*window->win_height)/tscr-3)-(window->win_height/(2*tscr))
					-(curpeak*sub_height/max)/2),
					peak_width, (curpeak*sub_height/max)/2);
			window->drawline(2+i*(window->win_width-4)/peak.ppeak_size+peak_width/2,
					((cscr*window->win_height)/tscr-3)-(window->win_height/(2*tscr))
					-(curamp*sub_height/maxamp)/2,
					2+i*(window->win_width-4)/peak.ppeak_size+peak_width/2,
					((cscr*window->win_height)/tscr-3)-(window->win_height/(2*tscr)),14);
		}  
	}
	if (title!=NULL) window->draw_text(title, 3, ((cscr-1)*window->win_height)/tscr+9, 2);    
	return 0;
}

char KVPlot::textplot(uint cscr, uint tscr, KVector& vector)
{
	window->drawframe(0, ((cscr-1)*window->win_height)/tscr,
                          window->win_width, window->win_height/tscr);
	
	char str[120]; 
	sprintf(str,"min:%3i avg:%3i max:%3i val:%3i std:%3lu",
	     vector.min(),
	     vector.mean(),
	     vector.max(),
	     vector.pvect[vector.pvect_size-1],
	     vector.std()	     	
	);
	window->draw_text(str, 3, ((cscr-1)*window->win_height)/tscr+10, 2);
	ves_t size; ve_t amp; vei_t lngth; bool sgn;
	vector.get_peak(&size, &amp, &lngth, &sgn);   
	sprintf(str,"psize:%5li pheight:%3i plen:%3i psign:%c",
	     size, (int)amp, lngth, (sgn==1)?'+':'-');
	window->draw_text(str, 3, ((cscr-1)*window->win_height)/tscr+22, 2); 
	return 0;
}

char KVPlot::labelplot(uint cscr, uint tscr, char* title, uint line,
                       uint col, uint font)
{
	window->drawframe(0, ((cscr-1)*window->win_height)/tscr,
                          window->win_width, window->win_height/tscr);
	if (title!=NULL) 
		window->draw_text(title, 3+(col*9),
                                  ((cscr-1)*window->win_height)/tscr+9*(line+1),
                                  2, font);    
	return 0;
}

char KVPlot::spiketrain(uint cscr, uint tscr, KVector& vector, int colour, 
                         char* title)
{
	window->drawframe(0, ((cscr-1)*window->win_height)/tscr,
                          window->win_width, window->win_height/tscr);
	

/*  
  uint sub_height = (uint)(win_height/tscr-4); 
 
  uint tot_height=(uint)((cscr*win_height)/tscr-3);
  for (vei_t i=1; i<vector.pvect_size; i++) {
    plot_point[i].x = 2+i*(win_width-14)/vector.pvect_size;
    plot_point[i].y = tot_height-(abs(vector.pvect[i]-
    vector.pvect[i-1])*sub_height)/255;
  } 
  // plot the line():
  XSetForeground(display, gc, plot_colours[colour].pixel);
  XDrawLines(display, buffer, gc, plot_point, 
             vector.pvect_size, CoordModeOrigin);
  // plot the stats:
  XDrawLine(display, buffer, gc, win_width-12, 
            tot_height-(vector.max()*sub_height)/255, win_width-3,
            tot_height-(vector.max()*sub_height)/255);
  XDrawLine(display, buffer, gc, win_width-12, 
            tot_height-(vector.min()*sub_height)/255,
            win_width-3,  tot_height-(vector.min()*sub_height)/255);
  XDrawLine(display, buffer, gc, win_width-12,
            tot_height-(vector.mean()*sub_height)/255,
            win_width-3,  tot_height-(vector.mean()*sub_height)/255);
  
  if (title!=NULL)
        draw_text(title, 3, ((cscr-1)*win_height)/tscr+9, 2);*/
  return 0;
}
  
char KVPlot::impulse(uint cscr, uint tscr, KVector& vector, int colour, 
                     char* title)
{
	window->drawframe(0, ((cscr-1)*window->win_height)/tscr,
                          window->win_width, window->win_height/tscr);
	
  /*
  uint sub_height = (uint)(win_height/tscr-4); 
  
  bucket_width = 1+(win_width-4)/vector.pvect_size;
  uint bar = 0, max = 255;
  XSetForeground(display, gc, plot_colours[colour].pixel);
  
  for (vei_t i=0;i<vector.pvect_size;i++) {
    // get the value of bucket i
     bar = vector.pvect[i];
    // plot current bar
    XFillRectangle(display, buffer, gc, 2+i*(win_width-4)/vector.pvect_size,
                   ((cscr*win_height)/tscr-3)-(bar*sub_height/max),
                   bucket_width, (bar*sub_height/max));
  }
  
  if (title!=NULL)
        draw_text(title, 3, ((cscr-1)*win_height)/tscr+9, 2);*/
  return 0;
}
