/***************************************************************************
                               knn.cpp v.2.01
                             -------------------
    begin                : Sept 23 2004
    copyright            : (C) 2004 by Martin Berchtold, Kristof Van Laerhoven
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
	first = current = NULL;
	kclasses = NULL;
	kdist = NULL;
	num_prototypes = 0;
}

KNN::KNN(vei_t k)
{
	knn = k;
	first = current = NULL;
	kclasses = new vei_t[knn];
	kdist = new f_32b[knn];
	num_prototypes = 0;
}

KNN::~KNN()
{
	if (kclasses!=NULL) delete []kclasses;
	if (kdist!=NULL) delete []kdist;
	// go through the dynamic list and delete the elements in the ptr list
	VectorPoCl *dummy;
	while (first!=NULL) {
		dummy = first;
		first = first->next;
		delete dummy;
	}
}

vei_t KNN::access(const VectorPoCl& datav, char dist, vei_t expo)
{  
	if (datav.vec_class == -1)
		return get_k_dis(datav, dist, expo); // get the k distances and vote
	else
	{  
		/*insert new vector at the beginning of the list with 
		start_pointer(t)=vector(t) and nextpointer(t)=start_pointer(t-1)*/
		VectorPoCl *dummy;
		dummy = new VectorPoCl(datav);
		dummy->next = first;
		first = dummy;
		num_prototypes++;
		return -1;
	}
}

inline
vei_t KNN::get_k_dis(const VectorPoCl& datav, char dist, vei_t expo)
{
   vei_t max;
	f_64b maxdis;
	max = 0;
	for (vei_t j=0; j<knn; j++) {
		kclasses[j] = -1;
		kdist[j] = max_oas;
	}
	/* determine the vector's class by going through the list
	   and fetch the k smallest distances*/
	
	if (first==NULL) return -1;
	
	current = first;
	
	while (current != NULL) // distances between vector and list vectors
	{  
		maxdis = 0;
		oas_t rad = det_rad(datav, dist, expo); //distance calculation
		if (rad < kdist[max]) {           // minimum distance?
				kdist[max] = rad;
				kclasses[max] = (*current).vec_class;
		}
		for (vei_t h=0; h<knn; h++) {      // update k nearest 
			if (kdist[h]>maxdis) {
				maxdis = kdist[h];
				max = h;
			}
		}
		current = (*current).next; //set pointer to next vector in list
	}
	
	//determine the maximum class in knn memory 
	vei_t maxclass = 0;
	for (vei_t i=0; i<knn; i++)
		if (kclasses[i]>maxclass)
			maxclass = kclasses[i];

	if (maxclass<1)  return -1;
	
	f_32b *numcl = new f_32b[maxclass+1]; // weigh & sum the classes..
	for (vei_t k=0; k<maxclass; k++)	numcl[k]=0.0; 
	allcl = 0;
	for (vei_t l=0; l<knn; l++) {
		if (kclasses[l]>-1) {
			numcl[kclasses[l]]+=(1.0/(kdist[l]+0.00001)); 
			allcl += (1.0/(kdist[l]+0.00001));
		}
	}
	
	// get the best class
	wincl=0;
	vei_t numclass=-1;
	for (vei_t m=0; m<=maxclass; m++)
		if (numcl[m] > wincl) {
			wincl = numcl[m];
			numclass = m;
		}
	delete []numcl;
	
	return numclass;
}

inline
f_32b KNN::det_rad(const VectorPoCl& datav, char seldis, vei_t exp)
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
