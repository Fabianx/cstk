/***************************************************************************
                                  knn.cpp
                             -------------------
    begin                : Sept 23 2004
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

#include "knn.h"

KNN::KNN()
{
	knn = 0;
	first = NULL;
	kclasses = NULL;
	kdist = NULL;
}

KNN::KNN(ve_t k)
{
	knn = k;
	kclasses = new ve_t[knn];
	kdist = new oas_t[knn];
	first = NULL;
}

KNN::~KNN()
{
	if (kclasses!=NULL) delete []kclasses;
	if (kdist!=NULL) delete []kdist;
}

vei_t KNN::access(VectorPoCl& datav, ve_t dist, vei_t expo)
{
	vei_t max=0;
	oas_t maxdis;
	vei_t numclass=-1;
	vei_t *numcl;
	vei_t maxclass;
	vei_t allcl=0;
	oas_t rad;
	if (datav.vec_class == -1)
	{
		/*determine the vectors class by going through the list
		and fetch the k smallest distances*/
		current = first;
		for (ve_t j=0; j<knn; j++)
		{
			kclasses[j] = 0;
			kdist[j] = 1000000000.0;
		}
		do //calculation for the distances between vector and list vectors
		{
			maxdis = 0.0;
			rad = det_rad(datav, dist, expo); //distance calculation
			if (rad < kdist[max])
					kdist[max] = rad;
					kclasses[max] = (*current).vec_class;
			for (ve_t h=0; h<knn; h++)
			{
				if (kdist[h]>maxdis)
				{
					maxdis = kdist[h];
					max = h;
				}
			}
			current = (*current).next; //set pointer to next vector in list
		}
		while (current != NULL);
		maxclass = 0;
		for (ve_t i=0; i<knn; i++)
		{
			//determine the maximum amount of classes in k-mem
			if (kclasses[i]>maxclass)
				maxclass = kclasses[i];
		}
		numcl = new vei_t[maxclass]; //vector for the klasses 
		for (ve_t l=0; l<knn; l++)
		{
			numcl[kclasses[l]]++;
		}

		for (ve_t m=1; m<=maxclass; m++)
		{
			//get the class with the most members
			if (numcl[m] > allcl)
			{
				allcl = numcl[m];
				numclass = m;
			}
		}
	}
	else
	{
		/*inserted vector at the beginning of the list with 
		start_pointer(t)=vector(t) and nextpointer(t)=start_pointer(t-1)*/ 
		datav.next = first;
		first = &datav;
	}
	return numclass;
}

oas_t KNN::det_rad(VectorPoCl& datav, ve_t seldis, vei_t exp)
{
	switch (seldis)
	{
		case DIS_MANH:	return datav.vector->dis_manh(*(current->vector));
		case DIS_CHEB:	return datav.vector->dis_cheb(*(current->vector));
		case DIS_EUCL:	return datav.vector->dis_eucl(*(current->vector));
		case DIS_MINK:	return datav.vector->dis_mink(*(current->vector), exp);
	}
	return datav.vector->dis_manh(*(current->vector));
}
