/***************************************************************************
                            dvector.h  -  v.1.10
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

#ifndef DVECTOR_H
#define DVECTOR_H

#include <stdio.h> // NULL, sprintf, ...
#include <math.h>
#include "vect_types.h"  // datacell
#include "datacell.h" 

#define abs(a) ((a)>0)?(a):-(a)

/**** DVector Definition ***************************************************
 * Use this class for input or data vectors for your algorithms. It        *
 * supports mixed types, and stores a pointer per component.               *
 ***************************************************************************/
class DVector {
 public:
   DVector();
   DVector(vei_t nsize);
   DVector(const DVector& vec);
   
   ~DVector(); 
   void create(vei_t nsize);
   void set_comp(f_64b value, char type, vei_t comp); 
   void set_comp(DataCell value, vei_t comp); // allocate a component
   f_64b get_comp(vei_t comp);       // return a component as a 64 bit float
   char  get_type(vei_t comp);                // return the type
   vei_t get_dim();                           // return the vector dimension
   oas_t dis_eucl(DVector& invec);            // Euclidean distance
   oas_t dis_manh(DVector& invec);            // Manhattan Distance
   oas_t dis_cheb(DVector& invec);            // Chebychev Distance
   oas_t dis_mink(DVector& invec, vei_t exp); // Minkowski Distance
   char* to_string(void);
   
   DVector& operator=(const DVector& vec); 
   DVector& operator+=(const DVector& vec);
   DVector& operator-=(const DVector& vec);
   friend DVector& operator+(const DVector& vec1, const DVector& vec2);
   friend DVector& operator-(const DVector& vec1, const DVector& vec2);
   friend oas_t operator*(const DVector& vec1, const DVector& vec2);
   friend DVector& operator*(const oas_t val, const DVector& vec);
 
 private:
   DVector& set(const DVector& vec, vei_t i);
   friend oas_t get(const DVector& vec, vei_t i);
 
   void **vct;                                // the pointer array
   char *types;                               // the types for each pointer
   vei_t vctsize;
   char *strout;                              // pointer to outpout buffer
};

#endif
