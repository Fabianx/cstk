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

KVPlot::KVPlot(){
}

KVPlot::~KVPlot(){
}

char KVPlot::histogram(uint cscr, uint tscr, KVector& vector, int colour, 
                        uint num_buckets, char* title){
  drawframe(0, ((cscr-1)*win_height)/tscr, win_width, win_height/tscr);
  // read each value for bar and draw it
  uint bucket_width = (win_width-4)/num_buckets;
  uint bar = 0, max = 0, sub_height;
  for (uint i=0;i<num_buckets;i++) {
    // get the fullest bucket i
     bar = vector.get_histo(num_buckets, i);
     if (max<bar)  max = bar;
  }
  XSetForeground(display, gc, plot_colours[colour].pixel);
  sub_height = (win_height/tscr-4-10);
  for (uint i=0;i<num_buckets;i++) {
    // get the value of bucket i
     bar = vector.get_histo(num_buckets, i);
    // plot current bar
    if (max == 0) max = 1; // Stop those lovely exceptions
    XFillRectangle(display, buffer, gc, 2+i*(win_width-4)/num_buckets,
                   ((cscr*win_height)/tscr-3)-(bar*sub_height/max),
                   bucket_width, (bar*sub_height/max)); 
  }          
  if (title!=NULL) draw_text(title, 3, ((cscr-1)*win_height)/tscr+9, 2);
  return 0;
}

char KVPlot::timeseries(uint cscr, uint tscr, KVector& vector, int colour, 
                         char* title, bool scaling){
  drawframe(0, ((cscr-1)*win_height)/tscr, win_width, win_height/tscr);
  uint sub_height = (uint)(win_height/tscr-4-10); 
  uint tot_height=(uint)((cscr*win_height)/tscr-3);
  uint scaling_fact=20;
  for (vei_t i=0; i<vector.pvect_size; i++) {
    plot_point[i].x = 2+i*(win_width-14)/vector.pvect_size;
    if (!scaling)
      plot_point[i].y = tot_height-(vector.pvect[i]*sub_height)/255;
    else {
      //plot_point[i].y = 
      // (short int) (tot_height -
      //  ((vector.pvect[i]-vector.min())*sub_height)
      //    /(0.1+vector.max()-vector.min()));
      if (vector.max()-vector.min()>20) 
           scaling_fact = vector.max()-vector.min();
      plot_point[i].y = 
        (short int) (tot_height-
        ((vector.pvect[i]-vector.min())*sub_height)/(0.1+scaling_fact));
    }
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

  if (title!=NULL) {
        draw_text(title, 3, ((cscr-1)*win_height)/tscr+9, 2);
  //    char str[3]; 
  //    sprintf(str,"%i",vector->pvect[vector->pvect_size-1]);
  //    draw_text(str, tot_height-(vector->min()*sub_height)/255, 
  //    win_width-20, 10, 2);
  }
  return 0;   
}
  
char KVPlot::peakplot(uint cscr, uint tscr, Peak& peak, char* title){ 
  drawframe(0, ((cscr-1)*win_height)/tscr, win_width, win_height/tscr);
  // read each value for bar and draw it
  pei_t num_peaks = peak.ppeak_size; 
  vei_t peak_width = (win_width-4)/num_peaks;
  ves_t curpeak = 0;
  ve_t  curamp = 0;
  ves_t max = 0;
  ve_t  maxamp = 0, sub_height= (win_height/tscr-4-20);
  for (pei_t i=0;i<num_peaks;i++) {
    // get the fullest bucket i
     curpeak = abs(peak.ppeak_s[i]);
     curamp  = abs(peak.ppeak_a[i]);
     if (max<curpeak)  max = curpeak;
     if (maxamp<curamp) maxamp = curamp;
  }
  if (max==0) max = 1;
  if (maxamp==0) maxamp = 1;
  for (pei_t i=0;i<num_peaks;i++) {
    // get the value of bucket i
     curamp = peak.ppeak_a[i];
     curpeak = ((peak.ppeak_p[i]==1)?1:-1)*peak.ppeak_s[i];
    // plot current bar
     if (curpeak<0) {
              XSetForeground(display, gc, plot_colours[2].pixel);
              XFillRectangle(display, buffer, gc, 2+i*(win_width-4)/num_peaks,
                             ((cscr*win_height)/tscr-3)-(win_height/(2*tscr)),
                             peak_width, (abs(curpeak)*sub_height/max)/2);
              XSetForeground(display, gc, plot_colours[14].pixel);
              XDrawLine(display, buffer, gc, 
                        2+i*(win_width-4)/num_peaks+peak_width/2,
                        ((cscr*win_height)/tscr-3)-(win_height/(2*tscr)),
                        2+i*(win_width-4)/num_peaks+peak_width/2,
                        ((cscr*win_height)/tscr-3)-(win_height/(2*tscr))
                         +(curamp*sub_height/maxamp)/2);              
     }
     else {
              XSetForeground(display, gc, plot_colours[1].pixel);
              XFillRectangle(display, buffer, gc, 2+i*(win_width-4)/num_peaks,
                             (((cscr*win_height)/tscr-3)-(win_height/(2*tscr))
                              -(curpeak*sub_height/max)/2),
                             peak_width, (curpeak*sub_height/max)/2);
              XSetForeground(display, gc, plot_colours[14].pixel);
              XDrawLine(display, buffer, gc, 
                        2+i*(win_width-4)/num_peaks+peak_width/2,
                        ((cscr*win_height)/tscr-3)-(win_height/(2*tscr))
                         -(curamp*sub_height/maxamp)/2,
                        2+i*(win_width-4)/num_peaks+peak_width/2,
                        ((cscr*win_height)/tscr-3)-(win_height/(2*tscr)));
     }  
  }
  if (title!=NULL) draw_text(title, 3, ((cscr-1)*win_height)/tscr+9, 2);     
  return 0;
}

char KVPlot::textplot(uint cscr, uint tscr, KVector& vector){
  
  drawframe(0, ((cscr-1)*win_height)/tscr, win_width, win_height/tscr);
  //uint sub_height = (uint)(win_height/tscr-4-10); 
  //uint tot_height=(uint)((cscr*win_height)/tscr-3);
  
	char str[120]; 
	sprintf(str,"min:%3i avg:%3i max:%3i val:%3i std:%3lu",
	     vector.min(),
	     vector.mean(),
	     vector.max(),
	     vector.pvect[vector.pvect_size-1],
	     vector.std()	     	
	);
	draw_text(str, 3, ((cscr-1)*win_height)/tscr+10, 2);
	ves_t size; ve_t amp; vei_t lngth; bool sgn;
	vector.get_peak(&size, &amp, &lngth, &sgn);   
	sprintf(str,"psize:%5li pheight:%3i plen:%3i psign:%c",
	     size, (int)amp, lngth, (sgn==1)?'+':'-'
	);
	draw_text(str, 3, ((cscr-1)*win_height)/tscr+22, 2);   
  
  return 0;
}

