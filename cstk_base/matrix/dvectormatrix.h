/***************************************************************************
                          dvectormatrix.h  -  v.0.1
                             -------------------
    begin                : Mo Oct 04 2004
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

#ifndef DVECTORMATRIX_H
#define DVECTORMATRIX_H

#include <stdio.h>
#include "cstk_base/vector/dvector.h"

/********************************************************************************************************
* 		List of DVector's with pointer to next element in list.					*
********************************************************************************************************/

class DVectorList
{  
	public:
		DVectorList() {vector=NULL;next=NULL;}
		DVectorList(vei_t nsize) {vector = new DVector(nsize);}
		~DVectorList() {if (vector) delete vector;}
		void create(vei_t nsize) {vector = new DVector(nsize);}
		DVector *vector;
		DVectorList *next;
};

/********************************************************************************************************
* 		DVectorMatrix definition that contains an array of DVector rows				*
*		Information of the number of rows are saved in vesize and the number of			* 
*		columns in hosize					 				*
********************************************************************************************************/

class DVectorMatrix
{
	public:
		DVectorMatrix();
		DVectorMatrix(vei_t hsize, vei_t vsize);
		DVectorMatrix(const DVectorMatrix& input);
		~DVectorMatrix();
		
		void create(vei_t hsize, vei_t vsize); 
		
		DVectorMatrix T();
		DVectorMatrix INV();
		oas_t det();
		
		DVectorMatrix& operator=(const DVectorMatrix& b); 
		
		friend DVectorMatrix& operator*(const DVectorMatrix& mat1, const DVectorMatrix& mat2_T); 
		friend DVector& operator*(const DVector& vec, const DVectorMatrix& mat_T);
		friend DVector& operator*(const DVectorMatrix& mat_T, const DVector& vec);
		
		friend DVectorMatrix& operator+(const DVectorMatrix& mat1, const DVectorMatrix& mat2); 
		friend DVectorMatrix& operator-(const DVectorMatrix& mat1, const DVectorMatrix& mat2); 
		
		DVector *vector;
		vei_t hosize, vesize;
		
	private:
		DVectorMatrix detinv(DVectorMatrix& mat);
		oas_t cofactor(DVectorMatrix& mat, vei_t i, vei_t j);
};

/********************************************************************************************************
* 		List of DVectorMatrices with pointer to next element in list, 				*
*		a DVector element associated with each Matrix and a integer variable			*
*		for class informations									*
********************************************************************************************************/

class DMatrixList
{
	public: 
		DMatrixList();
		DMatrixList(vei_t hsize, vei_t vsize);
		~DMatrixList();
		void create(vei_t hsize, vei_t vsize);
		DVectorMatrix *matrix;
		DMatrixList *next;
		DVector *mue;
		vei_t classinf;
};

#endif


