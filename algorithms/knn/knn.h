/***************************************************************************
                                    knn.h
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

#ifndef KNN_H
#define KNN_H

#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h"  

#define DIS_MANH 0
#define DIS_CHEB 1
#define DIS_EUCL 2
#define DIS_MINK 3

class VectorPoCl {  
	public:
		VectorPoCl() {vector=NULL;next=NULL;}
		VectorPoCl(vei_t nsize) {vector = new DVector(nsize);}
		~VectorPoCl() {if (vector) delete vector;}
		void create(vei_t nsize) {vector = new DVector(nsize);}
		DVector *vector;
		VectorPoCl *next;
		vei_t vec_class;
};

class KNN {
	private:
		ve_t knn;
		ve_t *kclasses;
		oas_t *kdist;
		
		VectorPoCl* first;
		VectorPoCl* current;
		
		oas_t det_rad(VectorPoCl& datav, ve_t seldis, vei_t exp);
		
	public:
		KNN();
		KNN(ve_t k);
		~KNN();
		
		vei_t access(VectorPoCl& datav, ve_t dist=DIS_EUCL, vei_t expo=2);
};

#endif

