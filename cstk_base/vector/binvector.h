/***************************************************************************
                            binvector.h  -  v.1.10
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

#ifndef BINVECTOR_H
#define BINVECTOR_H

/*************************************************************************** 
 * *@author Kristof Van Laerhoven                                          * 
 *                                                                         * 
 * BinVector is a class for binary vectors, that takes care of efficiently *
 * storing and retrieving the bits.                                        *
 ***************************************************************************/

#include <stdio.h> // NULL, sprintf, ...
#include "vect_types.h"

class BinVector {
 public:
   BinVector();
   BinVector(vei_t nsize);              // construct vector of size nsize
   ~BinVector();
   void  create(vei_t nsize);           // create binary vector of nsize 
   bool  get_comp(vei_t itr);           // get component
   bool  get_xor_comp(vei_t itr);       // get xorcomponent
   void  set_comp(bool val, vei_t itr); // set component
   void	 set_xor_comp(bool val, vei_t itr);  // set xor component
   void  tgl_comp(vei_t itr);           // toggle component
   vei_t get_dim();                     // return the vector's size
   vei_t dis_ham(BinVector& invec);	 // hamming distance (xor)
   vei_t dis_nand(BinVector& invec);	 // nand distance
   char* to_string(void);               // returns the vector as a string
 private:
   u_8b bmask(vei_t iter);              // byte mask
   u_8b *vect;
   u_8b *xorvec;                        // exclusive or vector
   char *strout;                        // pointer to outpout buffer
   vei_t size;
};

#endif // BINVECTOR_H
