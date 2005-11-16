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
	if (bucket!=NULL)
		delete []bucket;
	bucket = new DVector[buckets];
	bucket_num = buckets;
	alpval = alpha;
	selected_dist = seldist;
	exponent = exp;
}

void KMeans::savetoFile()
{
	FILE *f = fopen("cluster.sav","w");
	fprintf(f,"#clusters=%i, alphavalue=%f, distance=%u, exponent=%u, n=%i\n",
			bucket_num,alpval,selected_dist,exponent,bucket[0].get_dim());
	for (vei_t i=0; i<bucket_num; i++)
		for (vei_t j=0; j<bucket[i].get_dim(); j++)
			fprintf(f,"(%i,%i,%i)%lf ",i,j,bucket[i].get_type(j),bucket[i].get_comp(j));
	fclose(f);
}

int KMeans::restorefromFile()
{
	FILE *f = fopen("cluster.sav","r");
	if (!ferror(f))
	{
		int num, expo,n;
		float alph; 
		unsigned int dist;
		fscanf(f,"#clusters=%i, alphavalue=%f, distance=%u, exponent=%u, n=%i\n",&num,&alph,&dist,&expo,&n);
		//printf("#clusters=%i, alphavalue=%g, distance=%u, exponent=%u, n=%i\n",num,alph,dist,expo,n);
		create(num,alph,dist,expo);
		for (int i=0; i<num; i++)
			bucket[i].create(n);
		double out;	
		int i,j,t;
		while (!feof(f))
		{
			fscanf(f,"(%i,%i,%i)%lf ",&i,&j,&t,&out);
			//printf("(%i,%i,%i)%lf",i,j,t,out);
			bucket[i].set_comp(out,t,j);
		}
		fclose(f);
	}
	//printf("respective error occured: %i\n",ferror(f));
	return ferror(f);
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
	bucket[bnum] = bvec;
}

vei_t KMeans::update_bucket(DVector& vec)
{
	f_64b dis = 2*f_64b_max;
	vei_t elem=-1;
	for (vei_t j=0; j<bucket_num; j++)
	{
		if (det_rad(vec,bucket[j],selected_dist,exponent) < dis)
		{
			dis = det_rad(vec,bucket[j],selected_dist,exponent);
			elem = j;
		}		
	}
	if (elem!=-1)
		bucket[elem] += alpval * (vec - bucket[elem]);
	
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


