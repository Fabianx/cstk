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
  	SDMA(vei_t nsize, vei_t nasize, vei_t ndsize, oas_t thp=0);
  	~SDMA();
 	
	/********************************************************************************
	*	Method to create the SDMA with specification for the size of the 	*
	*	data and address vectors, size of the memory in entries and 		*
	*	threshold percentage for the distribution of the input data.		*
	********************************************************************************/
	void create(vei_t nsize, vei_t nasize, vei_t ndsize, oas_t thp=0);
	/********************************************************************************
	*	This method initializes the data and address vectors of the memory	*
	*	with random bits.							*
	********************************************************************************/
	void random_init();
	
	/********************************************************************************
	*	With this method address and data vector tupels are deleted from	*
	*	the memory according to the usage value.				*
	********************************************************************************/
	vei_t remove(oas_t usage);
	/********************************************************************************
	*	Method to store data vectors in the memory according to the address	*
	*	vector distances.							*	
	********************************************************************************/
 	vei_t store(BinVector& v1, BinVector& b1, bool det_radius);
	/********************************************************************************
	*	Method to retrieve data vectors out of the memory according to the 	*
	*	address	vector distances.						*
	********************************************************************************/
	vei_t retrieve(BinVector& v1, BinVector& tsum, 
	               BVector<oas_t>& tempsum, bool det_radius);

 protected:
 	BinVector      *av; // all the addresses are binary vectors
	BVector<oas_t> *av_tmp; // temporary data vectors
	BVector<oas_t> *dvd; // all the data vectors are vectors of counters
	
	// determines the smallest distance of input address and memory addresses
	// for relative distribution of the data vector
	vei_t radius(BinVector& v1);
	
	oas_t tmp;

	SDMA_PARAM par;
};

#endif 

