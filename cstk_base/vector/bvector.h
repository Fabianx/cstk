/***************************************************************************
                            bvector.h  -  v.1.10
                            --------------------
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

#ifndef BVECTOR_H
#define BVECTOR_H

#include <stdio.h> // NULL, sprintf, ...
#include <math.h> // pow
#include "vect_types.h" // atomic types

#define boolsign(a) (a>=0?1:0)
#define abs(a) ((a)>0)?(a):-(a)

/**** BVector Definition ***************************************************
 * Use this when you need a basic vector filled with values of the same    *
 * type (e.g., a data stream from one and the same sensor)                 *
 * The length of the vector is supposed to be fairly fixed, and works like *
 * a pipe (adding to the right shifts all values to the left).             *
 *                                                                         *
 * The constructor will need to specify what type you will be working with *
 * for instance: BVector <u_16b>v(20); // unsigned 16 bit                  *
 * or: BVectorList <f_32b>v; v.create(20); // 32 bit floating point        *
 ***************************************************************************/
template<class T> class BVector {  
 public:
   BVector();
   BVector(vei_t nsize);
   ~BVector();
   void   create(vei_t nsize);           // create the bvector with nsize
   void   add_comp(T newvalue);          // add value and shift left
   T      get_comp(vei_t iter);          // get the <iter>th component 
   vei_t  get_dim();                     // return the vector's size 
   T      min();                         // get minimum
   T      max();                         // get maximum
   T      mean();                        // get the average
   T      median();                      // get the median
   oas_t  rvar();                        // get the running variance
   oas_t  std();                         // get the standard deviation
   oas_t  var();                         // get the variance (classic way)
   T      val();                         // get the last component
   oas_t  get_histo(T buckets, T current); // create a histogram
   // Get the size, amplitude, length, and sign of the last peak:
   char   get_peak(oas_t *size, T *amp, vei_t *lngth, bool *sgn);
   char*  to_string(const char* format, int cell_len=10); 
   // (!) the following three functions will break the statistics: 
   void   set_comp(T value, vei_t iter); // set a specific component
   void   inc_comp(vei_t iter);          // increase component by 1 
   void   dec_comp(vei_t iter);          // decrease component by 1
   double dis_eucl(BVector<T>& invec); 	 // distance euklidian
   double dis_manh(BVector<T>& invec); 	 // distance manhatten
   double dis_cheb(BVector<T>& invec); 	 // distance chebychev
   double dis_mink(BVector<T>& invec, ve_t exp); // distance minkowski   
 private:
   void   qsort(vei_t start, vei_t stop);// quicksort for the median
   T*     comp;                          // the vector's components   
   vei_t  size;                          // the vector's size  
   T*     sorted;                        // sorted array, for median
   T      comp_min;                      // minimum so far
   T      comp_max;                      // maximum so far
   oas_t  rsum;                          // running sum of elements
   oas_t  rsqsum;                        // running square sum     
   char*  strout;                        // pointer to outpout buffer
};

#include "bvector.cpp"

#endif
