/***************************************************************************
                         rs232parser_t.cpp  -  v.1.00
                         ----------------------------
    begin                : Mar 05 2004
    copyright            : (C) 2004 by Kristof Van Laerhoven
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

#include "sensordata/rs232parser/rs232parser.h"
#include "viz/x11/kvplot.h"
 
int main(void) {
  
  int i=0, j=0;
    
  Rs232Parser pr(B38400,1024,"","/dev/tty.usbserial-ICNQM242");
  KVPlot kp;

  char buffer[24];
  bool quit=false;
  
  while (!quit) 
  {
	// user input
	 window->delay(0);
     temp_evnt = window->eventloop();
     if (temp_evnt==24)          // 'q'
        quit = true;      
    
	// read serial data in buffer
	 for (j=0; j<24; j++) buffer[j] = 0;
     pr.read(buffer);
	
	// extract vect from buffer
	 vect.add_comp(126); 
	
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
  }  
  
  return 0;
}
 


