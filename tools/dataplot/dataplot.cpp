/***************************************************************************
                           dataplot.cpp  -  v1.00
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

#include <stdlib.h>  //strcmp()
#include "kprof/kprof.h"
#include "../kgnuplot/kgnuplot.h"

#define FALSE 0
#define TRUE  1

int main(int ac, char **args) {

  Kgnuplot gp;
  KProf kprof;
  char debug=0;
  char tempstr[4096], tmpstr[255];
  unsigned int i;

  char * cmd="gnuplot\n";
  char * cmdn="gnuplot -persist\n";
  
  if (ac<2) {
    printf("\n DataPlot - by Kristof Van Laerhoven.");
    printf("\n syntax:");
    printf("\n   %s <xml settings document>",args[0]);
    printf("\n");
    printf("\n\n More info can be found in the CSTK manual (./manual/index.html). No really, read it.");
    printf("\n\n");
    exit(0);
  }

  if (ac>2) if ((args[2][0] == '-') &&(args[2][1] == 'd')) debug = TRUE;

  // Parse the xml file:
   if (kprof.parse(args[1],SETTP_DTP)) {printf("parse error\n\r");exit(1);};

  // send the preparation script to gnuplot:
   
   sprintf(tempstr,"set term x11\nset grid\nshow grid\nset multiplot\n" );
   gp.set_cmd(cmd, strlen(cmd));
   gp.set_script(tempstr, strlen(tempstr));
   gp.send_script();

   for (i=0; i<kprof.win.numids; i++) {          
          
          sprintf(tempstr,"%s set size 0.99,%f\nset origin 0.01,%f\n", tempstr,
                           0.99/(kprof.win.numids), 0.01+(kprof.win.numids-i-1)*(0.99/(kprof.win.numids)) );
                                                                          
          kprof.win.firstplot();
          do {
            if (i==kprof.win.get_id()) {
                // found another thing to plot:
                if (tempstr[strlen(tempstr)-1]=='\n') // first plot with this id?
                {
                   tmpstr[0]='\0'; // wipe
                   kprof.win.get_xtitle(tmpstr);
                   sprintf(tempstr,"%sset xlabel '%s'",tempstr,tmpstr);
                   kprof.win.get_ytitle(tmpstr);
                   sprintf(tempstr,"%s\nset ylabel '%s'",tempstr,tmpstr);
                   sprintf(tempstr,"%s\nplot ",tempstr);
                }
                // what is it?
                switch (kprof.win.get_type()) {
                 case PTYPE_TMSER: // if timeseries
                                  kprof.win.get_title(tmpstr); 
                                  sprintf(tempstr,"%s%c '%s' u %i t \"%s\" w l %i", tempstr,
                                                  (tempstr[strlen(tempstr)-1]!=' ')?',':' ',
                                                  kprof.is.filename, kprof.win.get_src()+1 ,tmpstr,
                                                  kprof.win.get_colour());
                                  break;
                 case PTYPE_IMPLS: // if impulseplot
                                  kprof.win.get_title(tmpstr);
                                  sprintf(tempstr,"%s%c '%s' u %i t \"%s\" w i %i", tempstr,
                                                  (tempstr[strlen(tempstr)-1]!=' ')?',':' ',
                                                  kprof.is.filename, kprof.win.get_src()+1 ,tmpstr,
                                                  kprof.win.get_colour());
                                  break;
                 case PTYPE_SCATT: // if scatterplot
                                  kprof.win.get_title(tmpstr);
                                  sprintf(tempstr,"%s '%s' u 1:2 t \"%s\" w p %i", tempstr,
                                                  kprof.is.filename, tmpstr,
                                                  kprof.win.get_colour());
                                  break;
                }
            }
          } while(kprof.win.nextplot());
          sprintf(tempstr,"%s\n",tempstr);
   }
   if (debug) printf("\n%s\n",tempstr);

   gp.set_cmd(cmdn, strlen(cmdn));
   gp.set_script(tempstr, strlen(tempstr));
   gp.send_script();

}



