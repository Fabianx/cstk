/***************************************************************************
                            topoplot.cpp  -  v1.00
                            ----------------------
    begin                : Mon Nov 24 2003
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
#include "viz/x11/clustplot.h"
#include "kprof/kprof.h"
#include "sensordata/udpparser/udpparser.h"
#include "sensordata/rs232parser/rs232parser.h"
#include "sensordata/logfileparser/logfileparser.h"
#include "sensordata/simparser/simparser.h"
#include <stdlib.h>  //strcmp()
#include "algorithms/ksom/ksom.h"   

int main(int ac, char **args) {

  char window_name[255];
  ClustPlot kp;                  // plots the cluster prototypes
  SensorData *sd = NULL;         // generic sensor data class
  KProf kprof;                   // read the profile from an XSD file
  KSOM ksom;
  int ret=0;
  bool quit=false;
  unsigned int i;
  char* channel_types = NULL;    // these variables are for passing 
  DataCell* columns=NULL;        // information along to the read methods
  uint* select=NULL;             // of the various types of sensordata
    
  if (ac<2) {
    printf("\n TopoPlot - by Kristof Van Laerhoven.");
    printf("\n syntax:");
    printf("\n   %s <xml settings document>",args[0]);
    printf("\n");
    printf("\n\n More info can be found in the CSTK manual");
    printf("\n\n (http://cstk.sf.net/). No really, read it.");
    printf("\n\n");
    exit(0);
  }
  
  // parse the xml file:
   if (kprof.parse(args[1],SETTP_TPP)) {printf("parse error\n\r");exit(1);};
   sprintf(window_name,"topoplot:%s,%s", imodes[kprof.input_mode],
           omodes[kprof.output_mode]);
   sprintf(window_name,"%s (%ix%i,%1.3f,%1.2f)", window_name, 
           kprof.par.gridsize[0], kprof.par.gridsize[1], kprof.par.lr, 
           kprof.par.nbr);
 
  // Create the window..
   kp.create(kprof.win.xpos, kprof.win.width, kprof.win.ypos, 
             kprof.win.height, kprof.win.border, window_name);
   kp.prepare_colours();
   
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
            sd = new Rs232Parser(kprof.is.rs232);
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
   
  // prepare the SOM:
    KVector vect(kprof.is.numcols,0); // no stats-keeping please
    ksom.createGrid( kprof.par.gridsize[0], kprof.par.gridsize[1], 
                     kprof.is.numcols);
    ksom.initRandom(25);
    
  // Loop until the task is interrupted:
   while (quit==false) {
       // do this several times to speed up viz:
        for (int i_step=0; i_step<kprof.win.skip; i_step++) {
            // read the sensordata: 
             if ( sd->read(channel_types, kprof.is.numchs, 
                           columns, select, kprof.is.numcols)) 
             { 
                for (i=0; i<kprof.is.numcols; i++) {
                       vect.add_comp(columns[i].get_u8b());
                } 
             } 
             ksom.feed_bell(&vect, kprof.par.lr, kprof.par.nbr);
        }
        
       // do the visualization:
         for (vei_t x=0; x<(vei_t)kprof.par.gridsize[0]; x++) 
         {
          for (vei_t y=0; y<(vei_t)kprof.par.gridsize[1]; y++) 
          {  
            ksom.getCell(x,y, &vect);
            switch (kprof.win.get_type()) 
	    {    
            case PTYPE_BARS:
                 kp.barplot(x,y, kprof.par.gridsize[0], kprof.par.gridsize[1],
                            &vect, ((ksom.winner_x==x)&&(ksom.winner_y==y))?8:15);
                 break;    
            case PTYPE_LINES:
                 kp.lineplot(x,y, kprof.par.gridsize[0], kprof.par.gridsize[1],
                            &vect, ((ksom.winner_x==x)&&(ksom.winner_y==y))?8:15 );
                 break;
            }     
          }
         }
        
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
          case 33: do {  //kp.swap_buffers();
                         kp.delay(kprof.win.framespersec);
                   } while (kp.eventloop()!=54);
                   break;
        }

   } // main while loop

  // release all allocated memory:
   if (sd!=NULL) delete sd;
   if (channel_types!=NULL) delete []channel_types;
   if (select!=NULL) delete []select;
   if (columns!=NULL) delete []columns;

  return 0;
}
