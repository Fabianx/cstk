/***************************************************************************
                           kvector_t.cpp  -  v.1.00
                           ------------------------
    begin                : Aug 17 2002
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

 /**KVector Analysis Demo
  *@author Kristof Van Laerhoven

   This demo shows what the KVector class can do - most of the explanations
   get printed out, so just run it and read.

 */

 #include "cstk_base/vector/kvector.h"
 #include <sys/time.h>     // for timings
 
 int main(void) {
  printf("testing the KVector routines:\n\r");
  vei_t i,j;
  KVector vect(5), vect2(5), vect3(5), vect5(1000);
  printf(" + Random %i-d vectors: ",vect.get_dim());
  for (i=0; i<5; i++) {
    vect.set_rand();
    printf("\n\r   (%s)", vect.to_string("%i "));
  }
  printf("\n\r + Using the vector as a sliding window (with stats):");
  for (i=0; i<5; i++) {
    vect.add_comp(i*33);
    printf("\n\r   (%s)", vect.to_string("%i "));
    printf("  --> min=%3i max=%3i mean=%3i median=%3i rvar=%5li var=%5li",
     vect.min(),vect.max(),vect.mean(),vect.median(),vect.rvar(),vect.var());
  }
  KVector vect4(15);
  vect4.set_rand();
  printf("\n\r + Histograms for (%s) :",vect4.to_string("%i "));
  for (i=2; i<11; i+=2) {
    printf("\n\r    %2i: ",i);
    for (j=0; j<i; j++) printf(" %3li", vect4.get_histo(i,j));
  }
  printf("\n\r + Peak detection:");
  vect4.set_rand();
  vect4.add_comp(12);  vect4.add_comp(180); vect4.add_comp(190); 
  vect4.add_comp(200); vect4.add_comp(210); vect4.add_comp(200); 
  vect4.add_comp(190); vect4.add_comp(180);
  ves_t psize;  ve_t pamp;  vei_t plen;  bool psgn;
  vect4.get_peak(&psize, &pamp, &plen, &psgn);
  printf("\n\r    (%s)\n\r    last peak ",vect4.to_string("%i ")); 
  printf("has size %li, amplitude %i, length %i, and is %s.(u=%i)",
    psize, pamp,plen,((psgn==1)?"positive":"negative"),vect4.mean());
  printf("\n\r + Timings:\n\r");
  struct timeval t1,t2;
  gettimeofday(&t1,0);
  for (i=0; i<10000; i++)
    vect.set_rand();
  gettimeofday(&t2,0);
  printf("    rand,5        = %f usec/inst\n\r",
      (float)((t2.tv_sec - t1.tv_sec)*1000000+(t2.tv_usec-t1.tv_usec))/10000 );
  gettimeofday(&t1,0);
  for (i=0; i<1000; i++)
    vect5.set_rand();
  gettimeofday(&t2,0);
  printf("    rand,1000     = %f usec/inst\n\r",
      (float)((t2.tv_sec - t1.tv_sec)*1000000+(t2.tv_usec-t1.tv_usec))/1000 );
  gettimeofday(&t1,0);
  for (i=0; i<10000; i++)
    vect.add_comp(128);
  gettimeofday(&t2,0);
  printf("    add_comp,5    = %f usec/inst\n\r",
      (float)((t2.tv_sec - t1.tv_sec)*1000000+(t2.tv_usec-t1.tv_usec))/10000 );
  gettimeofday(&t1,0);
  for (i=0; i<1000; i++)
    vect5.add_comp(128);
  gettimeofday(&t2,0);
  printf("    add_comp,1000 = %f usec/inst",
      (float)((t2.tv_sec - t1.tv_sec)*1000000+(t2.tv_usec-t1.tv_usec))/1000 );
  printf("\n\r + Limits:");
  printf("\n\r    maximum vector element:  %i",max_ve);
  printf("\n\r    maximum vector iterator: %i",max_vei);
  printf("\n\r    maximum vector sum:      %i\n\r\n\r",max_ves); 
 }
