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

/****************************************************************************************
*	The K-Means clustering clusters the input data in k different clusters.		*
****************************************************************************************/

class KMeans 
{
	public:
		KMeans(); //empty initialization
		//initialize values - specifies k, alpha(multipl. constant) and distance measurement
		KMeans(vei_t buckets, oas_t alpha, ve_t seldist=DIS_EUCL, vei_t exp=2); 
		~KMeans();
		
		//initialize values - specifies k, alpha(multipl. constant) and distance measurement
		void create(vei_t buckets, oas_t alpha, ve_t seldist=DIS_EUCL, vei_t exp=2); 
		
		void initialize(DVector& min, DVector& max); //initializes buckets vectors randomly
		void initialize(DVector& bvec, vei_t bnum);  //initializes buckets with input vector
		
		/********************************************************************************
		* 	This method updates the bucket with the smallest distance to the 	*
		*	input vector by a small portion of the difference between input 	*
		*	vector and bucket vector.						*
		********************************************************************************/
		vei_t update_bucket(DVector& vec); 
		/********************************************************************************
		* 	Method to retrieve the class information from the bucket with the 	*
		*	smallest distance to the input vector.					*
		********************************************************************************/
		vei_t retrieve_cluster(DVector& vec); 
		
		DVector* bucket;
		
	private:
		//returns the distance between the two input vectors according to the selected distance
		oas_t det_rad(DVector& vector, DVector& datav, ve_t seldis, vei_t exp);
	
		vei_t bucket_num, exponent;
		oas_t alpval;
		ve_t selected_dist;
};

#endif

