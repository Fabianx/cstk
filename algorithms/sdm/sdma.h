/***************************************************************************
                              sdma.h  -  v.1.00
                             -------------------
    begin                : Jun 01 2004
    copyright            : (C) 2002-2004 by Martin Berchtold
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

#ifndef SDMA_H
#define SDMA_H

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
  
*/

#include <stdlib.h>
#include "cstk_base/vector/binvector.h" // for addresses and data
#include "cstk_base/vector/bvector.h" // for the counters

//class SDMA : public SDM 
class SDMA
{
 public:
 	SDMA();
  	SDMA(vei_t nsize, vei_t nasize, vei_t ndsize, oas_t thp=0);
  	~SDMA();
 
	void create(vei_t nsize, vei_t nasize, vei_t ndsize, oas_t thp=0);
	void random_init();
	
	//void normal_init();
	//void init_store(BinVector& v1, BinVector& b1);
	vei_t remove(oas_t usage);
 	vei_t store(BinVector& v1, BinVector& b1, bool det_radius);
	vei_t retrieve(BinVector& v1, BinVector& tsum, 
	               BVector<oas_t>& tempsum, bool det_radius);

 private:
 
 protected:
 	BinVector      *av; // all the addresses are binary vectors
	BVector<oas_t> *av_tmp; // temporary data vectors
	BVector<oas_t> *dvd; // all the data vectors are vectors of counters
	
	vei_t radius(BinVector& v1);
	
	vei_t size;
  	vei_t asize;
  	vei_t dsize;
	
	oas_t tmp;

	oas_t thresholdp;
};

#endif // SDM_A

