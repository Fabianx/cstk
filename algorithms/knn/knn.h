/***************************************************************************
                               knn.h v.2.01
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

#ifndef KNN_H
#define KNN_H

#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h"  

#define DIS_MANH 0
#define DIS_CHEB 1
#define DIS_EUCL 2
#define DIS_MINK 3

/****************************************************************************************
*	Single linked DVector list element with copy constuctor and a vlue for 		*
*	class informations.								*
****************************************************************************************/

struct VectorPoCl {
		VectorPoCl() {vector=NULL;next=NULL;}
		VectorPoCl(vei_t nsize) {vector = new DVector(nsize);next = NULL;}
		VectorPoCl(const VectorPoCl& vect) 
		{
			vector = new DVector(*(vect.vector));
			next = vect.next;
		   	vec_class = vect.vec_class;   
		}
		~VectorPoCl() {if (vector) delete vector;}
		
		void create(vei_t nsize) {vector = new DVector(nsize);}
		DVector *vector;
		VectorPoCl *next;
		vei_t vec_class;
};

/****************************************************************************************
*	K neares neighbor algorithm that cunstructs a DVector list out of elements	*
*	with a class information and determines the class of elements where the		*
*	class value is not set. The class information is determined through the k	*
*	nearest neighbors in the list of classified elements.				*
****************************************************************************************/

class KNN {
	private:
		// number k of the neighbors which count
		ve_t knn;
		// pointer to the first element in the list
		VectorPoCl* first;
		// pointer to the currently observed element in the list
		VectorPoCl* current;
		
		/****************************************************************************************
		*	This method determines the distance between the DVectors in the classified	*
		*	list and the input DVector according to the selected vector distance		*
		*	measurement. The exponent value is used in the Minkowski distance measurement.	*
		****************************************************************************************/
		f_32b det_rad(const VectorPoCl& datav, char seldis, vei_t exp);
		
		vei_t *kclasses;     
		f_32b *kdist;
		f_32b wincl;   // last overall winner's weight
		f_32b allcl;   // sum of all winner's weight
		
	public:
		
		KNN();
		KNN(vei_t k);
		~KNN();
		
		/****************************************************************************************
		*	Main method which determines if the element should be saved in the list 	*
		*	or if the class information that is not yet set should be determined. If 	*
		*	the element should be saved it is done in this method by setting the next	*
		*	pointer of the element and the first pointer of the algorithm. Should the	*
		*	class information should be retrieved, the get_k_dis method is accessed.	* 
		****************************************************************************************/
		vei_t access(const VectorPoCl& datav, char dist=DIS_EUCL, vei_t expo=2);
		
		/****************************************************************************************
		*	This method determines the class of the input element according to its		*
		*	k nearest neighbors and returns the result. It is the actual implementation	*
		*	of the KNN algorithm.								*
		****************************************************************************************/
		vei_t get_k_dis(const VectorPoCl& datav, char dist=DIS_EUCL, vei_t expo=2);
		
		// returns the i'th class value of the k neighbours to the last input element
		vei_t get_class(vei_t i) { return (i<knn)?kclasses[i]:-1; };
		// returns the i'th distance value of the k neighbours to the last input element
		f_32b get_dist(vei_t i) { return (i<knn)?kdist[i]:-1; };
		// returns the confidence of the last retrieved class information
		f_32b get_conf() { return (wincl/allcl); };
		
		u_32b num_prototypes;
};

#endif

