/***************************************************************************
                           bvector_t.cpp  -  v.1.00
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

 /**BVector Analysis Demo
  *@author Kristof Van Laerhoven

   This demo shows what the BVector class can do - most of the explanations
   get printed out, so just run it and read.
 */

 #include "cstk_base/vector/bvector.h"

 int main(void) {
  printf("testing the BVector<> routines:\n\r");
  vei_t i;
  BVector<u_16b> a(3), b(3), c(3), d(5), e(8), f(8);
  // a = [12 8 14], b = [18 0 18], c = [28 3 58]
  // e = [1 180 190 200 210 200 190 180]
  a.set_comp(12,0);  a.set_comp(8,1);  a.set_comp(14,2);
  b.set_comp(18,0);  b.set_comp(0,1);  b.set_comp(18,2);
  c.set_comp(28,0);  c.set_comp(3,1);  c.set_comp(58,2);
  e.add_comp(1); e.add_comp(180); e.add_comp(190); e.add_comp(200);
  e.add_comp(210); e.add_comp(200); e.add_comp(190); e.add_comp(180);
  f.add_comp(23);   f.add_comp(234); f.add_comp(230); f.add_comp(243);
  f.add_comp(242); f.add_comp(14); f.add_comp(390); f.add_comp(14); 
  printf(" + a = (%s)\n\r", a.to_string("%i ") ); 
  printf(" + b = (%s)\n\r", b.to_string("%i ") ); 
  printf(" + Manhattan distance between the two = %f\n\r", a.dis_manh(b));
  printf(" + Euclidean distance = %f\n\r", a.dis_eucl(b));
  printf(" + Chebychev distance = %f\n\r", a.dis_cheb(b));
  printf(" + Minkowski distance = %f (l=4)\n\r", a.dis_mink(b,4));
  printf(" + Using the vector as a sliding window (with stats):");
  for (i=0; i<5; i++) {
    d.add_comp(i*33);
    printf("\n\r    (%s)", d.to_string("%3i ") );
    printf("  --> min=%3i max=%3i mean=%3i median=%3i rvar=%f var=%f",
        d.min(),d.max(),d.mean(),d.median(),(float)d.rvar(),(float)d.var());
  }
  printf("\n\r + Peak detection for (%s):", e.to_string("%3i "));
  oas_t esize;  u_16b eamp;  vei_t plen;  bool psgn;
  e.get_peak(&esize, &eamp, &plen, &psgn);
  printf("\n\r    [%s]\n\r   last peak ", e.to_string("%i ")); 
  printf("has size %f, amplitude %i, length %i, and is %s. (u=%i)",
         esize, eamp, plen, ((psgn==1)?"positive":"negative"),e.mean());
  printf("\n\r");
 } 

