/***************************************************************************
                            kvector.h  -  v.1.10
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

#ifndef KVECTOR_H
#define KVECTOR_H

#include <stdio.h> // NULL, sprintf, ...
#include <stdlib.h> // rand(), ...
#include <math.h>
#include "vect_types.h"  

#define boolsign(a) (a>=0?1:0)
#define abs(a) ((a)>0)?(a):-(a)

/**** KVector Definition ***************************************************
 * Use this class for visualisations on embedded devices (iPAQs etc.)      *
 * Also, you'll have to convert your values to unsigned 8bit values!       *
 ***************************************************************************/
class KVector {
public:
  KVector();
  KVector(vei_t size, bool stats=true);
  ~KVector();
  void  createVector(vei_t size); // create a vector specifying the size
  void  set_rand();               // randomize the vector
  void  add_comp(ve_t newvalue);  // add a value and shift left
  void  add_comp(ve_t newvalue, vei_t place); // this'll break stats!
  vei_t get_dim();                // return the vector size (or dimension)
  ve_t  min();                    // get minimum
  ve_t  max();                    // get maximum
  ve_t  mean();                   // get the average
  ve_t  median();                 // get the median
  ves_t rvar();                   // get the running variance
  ves_t std();                    // get the standard deviation
  ves_t var();                    // get the variance (the classic way)
  ve_t  val();                    // get the last component
  ves_t get_histo(ve_t buckets, ve_t current); // create a histogram
  // Get the size, amplitude, length, and sign of the last peak:
  char  get_peak(ves_t *size, ve_t *amp, vei_t *lngth, bool *sgn);
  char* to_string(const char* format, int cell_len=4); // vector2string
  double dis_eucl(KVector& invec); // distance euklidian
  double dis_manh(KVector& invec); // distance manhatten
  double dis_cheb(KVector& invec); // distance chebychev
  double dis_mink(KVector& invec, ve_t exp); // distance minkowski
public:
  vei_t pvect_size;
  ve_t* pvect;
private:
  void  qsort(vei_t start, vei_t stop);
  ve_t  histo[max_ve]; 	// histogram 0 - 255
  ve_t* pvect_sorted;  	// sorted array, for median
  ve_t  pvect_min;     	// minimum so far
  ve_t  pvect_max;     	// maximum so far
  ves_t pvect_rsum;    	// running sum of elements
  ves_t pvect_rsqsum;  	// running square sum of elements
  char* strout;        	// string buffer for output
};

#endif
