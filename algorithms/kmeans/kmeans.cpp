/***************************************************************************
                                  kmeans.cpp
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
 
#include <cstdlib>
#include <ctime>

#include "kmeans.h"

KMeans::KMeans()
{
	bucket = NULL;
}

KMeans::KMeans(vei_t buckets, oas_t alpha, ve_t seldist, vei_t exp)
{
	bucket = new DVector[buckets];
	bucket_num = buckets;
	alpval = alpha;
	selected_dist = seldist;
	exponent = exp;
}

KMeans::~KMeans()
{
	if (bucket!=NULL) delete []bucket;
}

void KMeans::create(vei_t buckets, oas_t alpha, ve_t seldist, vei_t exp)
{
	bucket = new DVector[buckets];
	bucket_num = buckets;
	alpval = alpha;
	selected_dist = seldist;
	exponent = exp;
}

void KMeans::initialize(DVector& min, DVector& max)
{
	for (vei_t i=0; i< bucket_num; i++)
	{
		for (vei_t j=0; j< min.get_dim(); j++)
		{
		
		}
	} 
}

void KMeans::initialize(DVector& bvec, vei_t bnum)
{
	if (bucket[bnum].get_dim() == 0)
		bucket[bnum].create(bvec.get_dim());
	for (vei_t i=0; i<bvec.get_dim(); i++)
		bucket[bnum].set_comp(bvec.get_comp(i), bvec.get_type(i), i);
}

vei_t KMeans::update_bucket(DVector& vec)
{
	f_96b dis = 2*f_96b_max;
	oas_t tmp;
	vei_t elem=-1;
	for (vei_t j=0; j<bucket_num; j++)
	{
		if (det_rad(vec,bucket[j],selected_dist,exponent) < dis)
		{
			dis = det_rad(vec,bucket[j],selected_dist,exponent);
			elem = j;
		}		
	}
	for (vei_t i=0; i<vec.get_dim(); i++)
	{
		if (elem!=-1)
		{
			tmp = (bucket[elem].get_comp(i)+alpval*
			      (vec.get_comp(i)-bucket[elem].get_comp(i)));
			bucket[elem].set_comp(tmp, F64B_TYPE, i);
		}
	}
	
	return elem;
}

vei_t KMeans::retrieve_cluster(DVector& vec)
{
	oas_t dis = 2*f_64b_max;
	vei_t elem=-1;
	for (vei_t j=0; j<bucket_num; j++)
	{
		if (vec.dis_eucl(bucket[j])<dis)
		{
			dis = vec.dis_eucl(bucket[j]);
			elem = j;
		}		
	}	
	
	return elem;
}

oas_t KMeans::det_rad(DVector& vector, DVector& datav, ve_t seldis, vei_t exp)
{
	switch (seldis)
	{
		case DIS_MANH:	return vector.dis_manh(datav);
		case DIS_CHEB:	return vector.dis_cheb(datav);
		case DIS_EUCL:	return vector.dis_eucl(datav);
		case DIS_MINK:	return vector.dis_mink(datav, exp);
	}
	return vector.dis_manh(datav);
}


