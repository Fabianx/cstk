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

#include "../kvector/kvector.h"
#include "../kssim/kssim.h"   // temp. for datafile-reading
#include "../kglplot/kglplot.h"
#include "../kudp/kudp.h"
#include "../kipi/kipi.h"
#include "../kprof/kprof.h"
#include "../ksom/ksom.h"   
#include <stdlib.h>  //strcmp()

#define FALSE 0
#define TRUE  1

int main(int ac, char **args) {

  char window_name[255];
  KGLplot kp;
  KIpi ipi_sens;
  KSSim ks;       // simulated sensors 
  KProf kprof;
  KUDP kudp(1500);   // port 1501 is the standard one
  KUDP kudp_s(1501);// idem dito, for getting udps - remeber that the port still can be changed
  KSOM ksom;
  char tmpstr[255];
  int ret=0, k, quit=FALSE, debug=FALSE, tmpval;
  unsigned int i,j;
  FILE *fp = NULL;
  sint *cols = NULL;
    
  if (ac<2) {
    printf("\n TopoPlot - by Kristof Van Laerhoven.");
    printf("\n syntax:");
    printf("\n   %s <xml settings document>",args[0]);
    printf("\n");
    printf("\n\n More info can be found in the CSTK manual (./manual/index.html). No really, read it.");
    printf("\n\n");
    exit(0);
  }
  if (ac>2) if ((args[2][0] == '-') &&(args[2][1] == 'd')) debug = TRUE;
  
  // parse the xml file:
   if (kprof.parse(args[1],SETTP_TPP)) {printf("parse error\n\r");exit(1);};
   sprintf(window_name,"topoplot:%s,%s", imodes[kprof.input_mode], omodes[kprof.output_mode]);
   sprintf(window_name,"%s (%ix%i,%1.3f,%1.2f)", window_name, kprof.par.gridsize[0],
                        kprof.par.gridsize[1], kprof.par.lr, kprof.par.nbr);
 
  // Now that we know what we need..
   kp.create(kprof.win.xpos, kprof.win.width, kprof.win.ypos, kprof.win.height, kprof.win.border, window_name);
   kp.prepare_colours();
   kp.init_timers();
   cols = new sint[kprof.is.numcols];
   kprof.is.firstcol();
   do {
      i = kprof.is.get_col_id();
      cols[i] = kprof.is.get_col_channel();
   } while (kprof.is.nextcol());   

  // set up the input:
   switch (kprof.input_mode) {
     case IMODE_RS232 : break;
     case IMODE_FILE  : ks.create(kprof.is.numcols);   // kssim contains a datafile reading function
                        break;
     case IMODE_PIPE  : break;
     case IMODE_UDP   : kudp_s.setPort(kprof.is.port); // send UDPs to specified port
                        switch (kudp_s.createGet()) {
                          case UDPERR_CANNOTBIND: printf("Error: Can't bind receive socket!\n\r"); break;
                          case UDPERR_CANNOTOPEN: printf("Error: Can't open receive socket!\n\r"); break;
                        }
                        break;
     case IMODE_SIM   : ks.create(kprof.is.numcols);   // the simulation just looks at the number of columns
                        break;
     case IMODE_INT   : break;
   }

  // set up the output:
   switch (kprof.output_mode) {
     case OMODE_RS232 : break;
     case OMODE_FILE  : fp = fopen(kprof.os.filename,"w"); break;
     case OMODE_PIPE  : break;
     case OMODE_UDP   : kudp.setPort(kprof.os.port);     // send UDPs to specified port
                        kudp.createSend(kprof.os.host);  // and host
                        break;
   }
    
  // prepare the SOM:
    KVector vect(kprof.is.numcols,0); // no stats-keeping please
    ksom.createGrid( kprof.par.gridsize[0], kprof.par.gridsize[1], kprof.is.numcols);
    ksom.initRandom(25);
    
  // Loop until the task is interrupted:
   while (quit==FALSE) {
     // get & output data (+ do this several times to speed up visualization):   
      for (int i_step=0; i_step<kprof.win.skip; i_step++) {           
        switch (kprof.input_mode) {
            case IMODE_RS232: ipi_sens.get_rs232(kprof.is.baudrate, kprof.is.serport, kprof.is.numchs, kprof.is.poll, kprof.is.bits);
                              for (i=0; i<kprof.is.numcols; i++) vect.add_comp(ipi_sens.sensor[i],i);
                              break;
            case IMODE_FILE:  ks.file_uchar(kprof.is.filename, cols, kprof.is.numcols);
                              for (i=0; i<kprof.is.numcols; i++) vect.add_comp(ks.sim[i],i);
                              break;
            case IMODE_PIPE:  break;
            case IMODE_UDP:   while (!kudp_s.getmsg(100)) {}      /*not healthy: change this*/
                              {
                                   i=0; j=0; k=0; ret=0;
                                   while (i<=strlen(kudp_s.msg)) {
                                      if ( (kudp_s.msg[i]<='9')&&(kudp_s.msg[i]>='0') ) {
                                        tmpstr[j] = kudp_s.msg[i]; j++; 
                                      } else {
                                        tmpstr[j] = '\0'; j=0; 
                                        if (sscanf(tmpstr,"%i",&tmpval)>0) {
                                          vect.add_comp(tmpval, ret); 
                                          for (unsigned int l=0; l<kprof.is.numchs;l++) 
					                  if (k==cols[l]) {ret++;break;}
                                          k++;
                                        }
                                      }
                                      i++;
                                   }
                              }
                              break;
            case IMODE_SIM:   ks.rand_nocorr_track_uchar(15, 240, 1, 10.0);
                              for (i=0; i<kprof.is.numcols; i++) vect.add_comp(ks.sim[i],i);
                              break;
            case IMODE_INT:   vect.add_comp(ipi_sens.get_light(),0);
                              break;
        }
        
        ksom.feed_bell(&vect, kprof.par.lr, kprof.par.nbr);

       // check then for the output type
        switch (kprof.output_mode) {
            case OMODE_RS232: break;
            case OMODE_FILE:  {
                                 kprof.os.firstch();
                                 do {
                                      i = kprof.os.get_src();
                                      switch (kprof.os.get_type()) {
                                        case OTYPE_MIN: fprintf(fp,"%i ", vect.min());  break;
                                        case OTYPE_MAX: fprintf(fp,"%i ", vect.max());  break;
                                        case OTYPE_AVG: fprintf(fp,"%i ", vect.mean()); break;
                                        case OTYPE_VAR: fprintf(fp,"%li ", vect.rvar()); break;
                                        case OTYPE_STD: fprintf(fp,"%li ", vect.std());  break;
                                        case OTYPE_VAL: fprintf(fp,"%i ", vect.val());  break;
                                      }
                                 } while (kprof.os.nextch());
                              }
                              fprintf(fp,"\n\r");
                              break;
            case OMODE_PIPE:  break;
            case OMODE_UDP:   kudp.msg[0]='\0'; // init
                              kprof.os.firstch();
                              do {
                                   i = kprof.os.get_src();
                                   switch (kprof.os.get_type()) {
                                        case OTYPE_MIN: sprintf(tmpstr,"%i ", vect.min());  break;
                                        case OTYPE_MAX: sprintf(tmpstr,"%i ", vect.max());  break;
                                        case OTYPE_AVG: sprintf(tmpstr,"%i ", vect.mean()); break;
                                        case OTYPE_VAR: sprintf(tmpstr,"%li ", vect.rvar()); break;
                                        case OTYPE_STD: sprintf(tmpstr,"%li ", vect.std());  break;
                                        case OTYPE_VAL: sprintf(tmpstr,"%i ", vect.val());  break;
                                   }
                                   strcat(kudp.msg,tmpstr);
                              } while (kprof.os.nextch());
                              kudp.sendmsg();
                              break;
        } // output mode switch
       
       }
       
       // do the visualization:
        {
	 for (vei_t x=0; x<(vei_t)kprof.par.gridsize[0]; x++) {
          for (vei_t y=0; y<(vei_t)kprof.par.gridsize[1]; y++) {
            ksom.getCell(x,y, &vect);
            kp.topoplot(x,y, kprof.par.gridsize[0], kprof.par.gridsize[1],
                             &vect, ((ksom.winner_x==x)&&(ksom.winner_y==y))?8:15 );
          }
         }
        } 

       // swap buffers and delay a while:
          kp.swap_buffers();
          kp.delay(kprof.win.framespersec);

       // interruption?
        switch (ret=kp.eventloop()) { // press 'q' for quit
          case 24: quit = TRUE;
                   break;
          case 82: if (kprof.win.skip>0) kprof.win.skip--;
                   break;
          case 86: if (kprof.win.skip<255) kprof.win.skip++;
                   break;
          case 33: do {//kp.swap_buffers();
	               kp.delay(kprof.win.framespersec);} while (kp.eventloop()!=54);
                   break;
        }

   } // main while loop

  if (cols!=NULL) delete []cols;
  if (fp!=NULL) fclose(fp);

  
  return 0;  
}
