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

struct VectorPoCl {
		VectorPoCl() {vector=NULL;next=NULL;}
		VectorPoCl(vei_t nsize) {vector = new DVector(nsize);next = NULL;}
		VectorPoCl(const VectorPoCl& vect) {
			vector = new DVector(vect.vector->get_dim());
			for (vei_t i=0; i<vect.vector->get_dim();i++) 
			 vector->set_comp(vect.vector->get_comp(i), vect.vector->get_type(i),i);
			next = vect.next;
		   vec_class = vect.vec_class;   
		}
		~VectorPoCl() {if (vector) delete vector;}
		void create(vei_t nsize) {vector = new DVector(nsize);}
		DVector *vector;
		VectorPoCl *next;
		vei_t vec_class;
};

class KNN {
	private:
		ve_t knn;
		VectorPoCl* first;
		VectorPoCl* current;
		f_32b det_rad(const VectorPoCl& datav, char seldis, vei_t exp);
		vei_t *kclasses;     
		f_32b *kdist;
		f_32b wincl;   // last overall winner's weight
		f_32b allcl;   // sum of all winner's weight
		
	public:
		
		KNN();
		KNN(vei_t k);
		~KNN();
		
		vei_t access(const VectorPoCl& datav, char dist=DIS_EUCL, vei_t expo=2);
		vei_t get_k_dis(const VectorPoCl& datav, char dist=DIS_EUCL, vei_t expo=2);
		vei_t get_class(vei_t i) { return (i<knn)?kclasses[i]:-1; };
		f_32b get_dist(vei_t i) { return (i<knn)?kdist[i]:-1; };
		f_32b get_conf() { return (wincl/allcl); };
		
		u_32b num_prototypes;
};

#endif

