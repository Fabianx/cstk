/***************************************************************************
                           binvector_t.cpp  -  v.1.00
                           --------------------------
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

 /**BinVector Analysis Demo
  *@author Kristof Van Laerhoven

   This demo shows what the binvector class can do - most of the 
   explanations get printed, so just run it and read.
 */

 #include "cstk_base/vector/binvector.h"
 #include <sys/time.h>     // for timings

 int main(void) {
 
  BinVector v,u;
  v.create(17);
  //u.create(17);
  BinVector w(17);
  
  printf("testing the BinVector<> routines:\n\r");
  v.set_comp(1,2);    v.set_comp(1,11);  
  v.set_comp(1,12);   v.set_comp(1,13); 
  w.tgl_comp(3);      w.tgl_comp(11);  
  w.tgl_comp(12);     w.tgl_comp(13); 
  w.tgl_comp(5);      w.tgl_comp(7); 
  printf(" + v = %s\n\r", v.to_string());
  printf(" + w = %s\n\r", w.to_string());
  printf(" + Hamming distance between the two: %i\n\r\n\r", v.dis_ham(w));
  u = v && w;
  printf(" + AND between the two: %s\n\r\n\r", u.to_string());
  u = v & w;
  printf(" + AND between the two: %s\n\r\n\r", u.to_string());
  u = v || w;
  printf(" + OR between the two: %s\n\r\n\r", u.to_string());
  u = v | w;
  printf(" + OR between the two: %s\n\r\n\r", u.to_string());
  printf(" + v == w ? %i\n\r\n\r", (int)(v == w));
  printf(" + u == u ? %i\n\r\n\r", (int)(u == u));
  return 0;
  
 }
