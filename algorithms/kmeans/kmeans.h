/***************************************************************************
                                    kmeans.h
                             -------------------
    begin                : Sept 28 2004
    copyright            : (C) 2004 by Martin Berchtold
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
 
#ifndef KMEANS_H
#define KMEANS_H

#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h"  

#define DIS_MANH 0
#define DIS_CHEB 1
#define DIS_EUCL 2
#define DIS_MINK 3

class KMeans 
{
	public:
		KMeans(); //empty initialization
		//initialize values (specifies k and type of initialization):
		KMeans(vei_t buckets, oas_t alpha, ve_t seldist=DIS_EUCL, vei_t exp=2); 
		~KMeans();
		
		//initialize values (specifies k and type of initialization):
		void create(vei_t buckets, oas_t alpha, ve_t seldist=DIS_EUCL, vei_t exp=2); 
		
		void initialize(DVector& min, DVector& max); //initializes buckets randomly
		void initialize(DVector& bvec, vei_t bnum); // initializes buckets
		
		vei_t update_bucket(DVector& vec); //update bucket with smallest distance
		vei_t retrieve_cluster(DVector& vec);
		
		DVector* bucket;
		
	private:
		oas_t det_rad(DVector& vector, DVector& datav, ve_t seldis, vei_t exp);
	
		vei_t bucket_num, exponent;
		oas_t alpval;
		ve_t selected_dist;
};

#endif

