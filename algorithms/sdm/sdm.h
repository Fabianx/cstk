/***************************************************************************
                              sdm.h  -  v.1.00
                             -------------------
    begin                : Jun 01 2004
    copyright            : (C) 2002-2004 by Kristof Van Laerhoven
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

#ifndef SDM_H
#define SDM_H

/**Class for Creating and Maintaining a Sparse Distributed Memory
  *@author Kristof Van Laerhoven

  Sparse Distributed Memory (SDM) as described by Kanerva is pretty simple:

  - storing happens by updating counters in the data memories in the 
    neighbouring SDM addresses:
     e.g., if we want to store '0110101' in '101101' then we'll update the 
     counters of neighbours such as 001101, 101001, and 101111 by:
     -1 +1 +1 -1 +1 -1 +1
  - retrieving patterns happens by content: select all the neighbours for 
    the data pattern, and sum up their counters. After thresholding, the
    final vector should be close or equal to the written one.

  Main functionality:   
   
  * creating the memory:  
      SDM( nsize, nasize, ndsize, th) or create( nsize, nasize, ndsize, th)      
      with:
        nsize the number of addresses
	nasize the length (in bits) of the addresses
	ndsize the length (in bits) of the data
	th the radius threshold (usually 5-10) 
	
      random_init() creates the addresses randomly

  * storing in memory:
      store( &vaddress, &vdata)
      with both address and data BinVectors
      
  * retrieving the data:
      retrieve( &vaddress, &vdata, &sum)
      with both vaddress and vdata BinVectors, the result will be placed
      in vdata. sum will be the vector with the sums (for verification).
  
*/

#include <stdlib.h>
#include "cstk_base/vector/binvector.h" // for addresses and data
#include "cstk_base/vector/bvector.h" // for the counters

class SDM 
{
 public:
  	SDM();
  	SDM(vei_t nsize, vei_t nasize, vei_t ndsize, vei_t th=0);
  	~SDM();
  
  	void  random_init();
  	void  create(vei_t nsize, vei_t nasize, vei_t ndsize, vei_t th=0);
  	vei_t store(BinVector& v1, BinVector& b1, vei_t th); 
  	vei_t store(BinVector& v1, BinVector& b1);           
  	vei_t retrieve(BinVector& v1, BVector<s_16b>& tsum, vei_t th); 
  	vei_t retrieve(BinVector& v1, BinVector& tsum, BVector<s_16b>& tempsum); 
  
 private:
  	BinVector      *av; // all the addresses are binary vectors
  	BVector<s_16b> *dv; // all the data vectors are vectors of counters
  	vei_t size;
  	vei_t asize;
  	vei_t dsize;
  	vei_t threshold;
};

#endif // SDM_H
