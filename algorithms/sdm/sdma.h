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

#include <stdlib.h>
#include "cstk_base/vector/binvector.h" // for addresses and data
#include "cstk_base/vector/bvector.h" // for the counters

typedef struct SDMA_PARAM
{
	// size of the memory in data and address vector tupels
	vei_t size;
	// size of the address vectors
  	vei_t asize;
	// size of the data vectors
  	vei_t dsize;
	// threshold percentage for distribution of the data according to the distance 
	// of the address vectors
	oas_t thresholdp;
	// threshold for SOSDM
	vei_t threshold;
};

/****************************************************************************************
*	Sparse Distributed Memory Approach implementation that distributes the 		*
*	input data vector weighted over the whole memory.				*
****************************************************************************************/
class SDMA
{
 public:
 	SDMA();
  	SDMA(SDMA_PARAM param);
  	~SDMA();
 	
	void create(SDMA_PARAM param);

	void random_init();
	
	vei_t remove(oas_t usage);
 	vei_t store(BinVector& v1, BinVector& b1, bool det_radius);
	vei_t retrieve(BinVector& v1, BinVector& tsum, 
	               BVector<oas_t>& tempsum, bool det_radius);

 protected:
 	BinVector      *av; // all the addresses are binary vectors
	BVector<oas_t> *av_tmp; // temporary data vectors
	BVector<oas_t> *dvd; // all the data vectors are vectors of counters
	
	vei_t radius(BinVector& v1);
	
	oas_t tmp;

	SDMA_PARAM par;
};

#endif 

