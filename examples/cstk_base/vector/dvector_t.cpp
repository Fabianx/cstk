/***************************************************************************
                           dvector_t.cpp  -  v.1.00
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

 /**DVector Analysis Demo
  *@author Kristof Van Laerhoven

   This demo shows what the DVector class can do - most of the explanations
   get printed out, so just run it and read.

 */

 #include "cstk_base/vector/dvector.h"
 #include <sys/time.h>     // for timings
 
 int main(void) {
  printf("testing the DVector routines:\n\r");
  DVector v(4), w(4);
  DataCell dummy;
  dummy.set_u8val(150);      v.set_comp(dummy,0); 
  dummy.set_s32val(199520);  v.set_comp(dummy,1);
  dummy.set_f32val(15.2458); v.set_comp(dummy,2);
  dummy.set_f64val(-15878);  v.set_comp(dummy,3);
  w.set_comp(136,     U8B_TYPE, 0); 
  w.set_comp(199481,  S32B_TYPE,1); 
  w.set_comp(14.2914, F32B_TYPE,2); 
  w.set_comp(-16122,  F64B_TYPE,3); 
  printf("  + v = (%s)\n\r", v.to_string());
  printf("  + w = (%s)\n\r", w.to_string());
  printf("  + manhattan distance = %g\n\r", w.dis_manh(v));
  printf("  + Euclidean distance = %g\n\r", w.dis_eucl(v));
  printf("  + Chebychev distance = %g\n\r", w.dis_cheb(v));
  printf("  + Minkowski distance = %g (l=5)\n\r", w.dis_mink(v,5));
  printf("\n\r");
 }
