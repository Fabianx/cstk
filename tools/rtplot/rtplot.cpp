/***************************************************************************
                          rtplot.cpp  -  v.1.10
                          ---------------------
    begin                : Fri Nov 14 2003
    copyright            : (C) 2003 by Kristof Van Laerhoven
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

#include "cstk_base/vector/kvector.h"
#include "algorithms/peak/peak.h"
#include "viz/x11/kvplot.h"
#include "kprof/kprof.h"
#include "sensordata/udpparser/udpparser.h"
#include "sensordata/rs232parser/rs232parser.h"
#include "sensordata/logfileparser/logfileparser.h"
#include "sensordata/simparser/simparser.h"
#include <stdlib.h>  //strcmp()

int main(int ac, char **args) {
  
  if (ac<2) {
    printf("\n RTPlot - by Kristof Van Laerhoven.");
    printf("\n syntax:");
    printf("\n   %s <xml settings document>",args[0]);
    printf("\n");
    printf("\n\n More info can be found in the CSTK manual");
    printf("\n\n (http://cstk.sf.net/). No really, read it.");
    printf("\n\n");
    exit(0);
  }
  
  char window_name[255];
  KVPlot kp;                     // X11 plotting of KVectors
  KVector *vect = NULL;          // KVector array for the vectors
  Peak *peak = NULL;             // the class that stores signal's peaks
  Pk pk;                         // for passing raw peak attributes 
  SensorData *sd = NULL;         // generic sensor data class
  KProf kprof;                   // read the profile from an XSD file
  char tmpstr[255];              // temporary string
  unsigned int i;
  int ret;
  char quit=false;
  char* channel_types = NULL;    // these variables are for passing 
  DataCell* columns=NULL;        // information along to the read methods
  uint* select=NULL;             // of the various types of sensordata
  
  // Prepare the Plotting Window:
    int res = kprof.parse(args[1],SETTP_RTP);
    
    if (res!=0) {
       printf("Parse error in file '%s' on line %i.\n\r", args[1], res);
       exit(1);
    }
    sprintf(window_name,"rtplot:%s,%s", imodes[kprof.input_mode], 
                                        omodes[kprof.output_mode]);
   
  // Create the window..
   kp.create(kprof.win.xpos, kprof.win.width, kprof.win.ypos, 
             kprof.win.height, kprof.win.border, window_name);
   kp.prepare_colours();
  
  // fill in the input channel and columns arrays
   channel_types = new char[kprof.is.numchs];
   select = new uint[kprof.is.numcols];
   columns = new DataCell[kprof.is.numcols];
   kprof.is.firstch(); i=0;
   do {
       channel_types[i] = DC_typecast(kprof.is.get_sign(), 
                                      kprof.is.get_bits(),
                                      kprof.is.get_format());
       i++;
   } while (kprof.is.nextch());
   kprof.is.firstcol(); i=0;
   do {
       select[i] = kprof.is.get_col_channel();
       columns[i].set_type(channel_types[select[i]]);
       columns[i].set_bits(kprof.is.get_col_bits());
       i++; 
   } while (kprof.is.nextcol());
     
  // set up the input:
   switch (kprof.input_mode) {
     case IMODE_RS232 : 
            sd = new Rs232Parser(*kprof.is.rs232);
            break;
     case IMODE_FILE  : 
            sd = new LogFileParser(kprof.is.filename); 
            break;
     case IMODE_UDP   : 
            sd = new UDPParser(kprof.is.port, 100);
            break;
     case IMODE_SIM   : 
            sd = new SimParser(kprof.is.numcols);
            break;
     case IMODE_PIPE  : break;
     case IMODE_INT   : break;
   }
     
  // set up the output:
   switch (kprof.output_mode) {
     case OMODE_RS232 : break;
     case OMODE_FILE  : break;
     case OMODE_PIPE  : break;
     case OMODE_UDP   : break;
   }
  
  // create a vector and peak per column:
   vect = new KVector[kprof.is.numcols];
   peak = new Peak[kprof.is.numcols];
   kprof.is.firstcol();
   do {
     //fill in the resolutions:
      i = kprof.is.get_col_id();
      res = kprof.win.get_res(i, PTYPE_TMSER); // these are all zero,
      res+= kprof.win.get_res(i, PTYPE_TRAIN); // except for the one valid  
      res+= kprof.win.get_res(i, PTYPE_IMPLS); // plot-type
      vect[i].createVector(res);    
      peak[i].createPeak(kprof.win.get_res(i, PTYPE_PEAKS));
   } while (kprof.is.nextcol());
  
  // Main Loop until the task is interrupted:
   while (quit==false) {
       // do this several times to speed up viz:
        for (int i_step=0; i_step<kprof.win.skip; i_step++) {
            // read the sensordata: 
	    int ret=sd->read(channel_types, kprof.is.numchs, 
                           columns, select, kprof.is.numcols);
             if ( ret < 0 ) {
			    if (ret!=-12) printf("Error: %i\n",-ret);
			 }
		     else
                for (i=0; i<kprof.is.numcols; i++) {
                       vect[i].add_comp(columns[i].get_u8b());
		        }
            // update peaks according to the plot:
             kprof.win.firstplot();
             do {
               if (kprof.win.get_type()==PTYPE_PEAKS) {
                 vect[kprof.win.get_src()].get_peak(&pk.s, &pk.a, &pk.l, &pk.p);
                 peak[kprof.win.get_src()].add_peak(&pk);
               }   
             } while (kprof.win.nextplot());
            // check then for the output type:
        } // for loop for the skip steps  
       
       // do the visualization:
        kprof.win.firstplot();
        do { 
            switch (kprof.win.get_type()) {
                 case PTYPE_HISTO: // if histogram
                       kprof.win.get_title(tmpstr);
                       kp.histogram(kprof.win.get_id()+1, kprof.win.numids,
                                    vect[kprof.win.get_src()],
                                    kprof.win.get_colour(), 
                                    kprof.win.get_res(), tmpstr);
                       break;
                 case PTYPE_TMSER: // if timeseries
                       kprof.win.get_title(tmpstr);
                       kp.timeseries(kprof.win.get_id()+1, kprof.win.numids,
                                     vect[kprof.win.get_src()],
                                     kprof.win.get_colour(),
                                     tmpstr, kprof.win.get_scale());
                       break;
                 case PTYPE_PEAKS: // if peakplot
                       kprof.win.get_title(tmpstr);
                       kp.peakplot(kprof.win.get_id()+1, kprof.win.numids,
                                   peak[kprof.win.get_src()], tmpstr);
                       break;
                 case PTYPE_TEXT:  // if textplot
                       kp.textplot(kprof.win.get_id()+1, kprof.win.numids,
                                   vect[kprof.win.get_src()] );
                       break;
                 case PTYPE_TRAIN: // if spiketrain
                       kprof.win.get_title(tmpstr);
                       kp.spiketrain(kprof.win.get_id()+1, kprof.win.numids,
                                     vect[kprof.win.get_src()],
                                     kprof.win.get_colour(), tmpstr);
                       break;
                 case PTYPE_IMPLS: // if impulse plot
                       kprof.win.get_title(tmpstr);
                       kp.impulse(kprof.win.get_id()+1, kprof.win.numids,
                                  vect[kprof.win.get_src()],
                                  kprof.win.get_colour(), tmpstr);
                       break;
            }
        } while(kprof.win.nextplot()); 

       // swap buffers and delay a while:
          kp.swap_buffers();
          kp.delay(1000000/kprof.win.framespersec);

       // interruption?
        switch (ret=kp.eventloop()) { // press 'q' for quit
          case 24: quit = true;
                   break;
          case 82: if (kprof.win.skip>0) kprof.win.skip--;
                   break;
          case 86: if (kprof.win.skip<255) kprof.win.skip++;
                   break;
          case 33: do 
                   {   //kp.swap_buffers();
                       kp.delay(1000000/kprof.win.framespersec);
                   } while (kp.eventloop()!=54);
                   break;
        }
   } // Main Loop
    
  // release all allocated memory:
   if (sd!=NULL) delete sd;
   if (channel_types!=NULL) delete []channel_types;
   if (select!=NULL) delete []select;
   if (vect!=NULL) delete []vect;
   if (peak!=NULL) delete []peak;
   if (columns!=NULL) delete []columns;
  return 0;
}
