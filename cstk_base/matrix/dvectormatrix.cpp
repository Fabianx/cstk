/***************************************************************************
                          dvectormatrix.cpp  -  v.0.1
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
 
 #include "dvectormatrix.h"
 
/********************************************************************************************************
* 		DVectorMatrix definition that contains an array of DVector rows				*
*		Information of the number of rows are saved in vesize and the number of			* 
*		columns in hosize					 				*
********************************************************************************************************/
 
DVectorMatrix::DVectorMatrix() 
{
 	vector=NULL;
}
 
 DVectorMatrix::DVectorMatrix(vei_t hsize, vei_t vsize) 
{
	hosize = hsize;
	vesize = vsize;
	
	vector = new DVector[vsize];
	for (vei_t i=0; i<vsize; i++)
		vector[i].create(hsize);
}

 DVectorMatrix::DVectorMatrix(const DVectorMatrix& input)
{
	hosize = input.hosize;
	vesize = input.vesize;
	
	vector = new DVector[vesize];
	for (vei_t i=0; i<vesize; i++)
		vector[i].create(hosize);
		
	for (vei_t i=0; i<vesize; i++)
		for (vei_t j=0; j<hosize; j++)
			vector[i].set_comp(input.vector[i].get_comp(j), input.vector[i].get_type(j), j); 
}

DVectorMatrix::~DVectorMatrix() 
{
	if (vector!=NULL) delete []vector;
} 
		
void DVectorMatrix::create(vei_t hsize, vei_t vsize) 
{
	vector = new DVector[vsize];
	for (vei_t i=0; i<vsize; i++)
		vector[i].create(hsize);
}

DVectorMatrix DVectorMatrix::T()
{
	DVectorMatrix *matrix_T;
	matrix_T = new  DVectorMatrix(vesize, hosize);
	
	for (vei_t i=0; i<matrix_T->vesize; i++)
	{
		for (vei_t j=0; j<matrix_T->hosize; j++)
		{
			matrix_T->vector[i].set_comp(vector[j].get_comp(i), vector[j].get_type(i), j);
		}
	}
	return *matrix_T;
}

DVectorMatrix DVectorMatrix::INV()
{
	DVectorMatrix *matrix_inv;
	matrix_inv = new  DVectorMatrix(*this);
	
	bool succ = true;
	
 	for (vei_t k = 0; k < matrix_inv->vesize; k++)
	{
		if (matrix_inv->vector[k].get_comp(k)==0)
		{
			//printf("devision with 0? value_%i_%i=%7.1e\n\r",k,k,matrix_inv->vector[k].get_comp(k));
			succ = false;
			break;
		}
		else
			matrix_inv->vector[k].set_comp(((-1)/matrix_inv->vector[k].get_comp(k)), F64B_TYPE, k);
			
		for (vei_t i = 0; i < matrix_inv->vesize; i++)
			if (i != k)		
				matrix_inv->vector[i].set_comp(matrix_inv->vector[i].get_comp(k)*matrix_inv->vector[k].get_comp(k),F64B_TYPE, k);
				
		for (vei_t i = 0; i < matrix_inv->vesize; i++)
			if (i != k)
				for (vei_t j = 0; j < matrix_inv->vesize; j++)	
					if (j != k)
						matrix_inv->vector[i].set_comp((matrix_inv->vector[i].get_comp(j)+(matrix_inv->vector[i].get_comp(k)*matrix_inv->vector[k].get_comp(j))), F64B_TYPE, j);
		
		for (vei_t i = 0; i < matrix_inv->vesize; i++)
			if (i != k)
				matrix_inv->vector[k].set_comp(matrix_inv->vector[k].get_comp(i)*matrix_inv->vector[k].get_comp(k),F64B_TYPE, i);
	}
	
	if (succ)
	{
		for (vei_t k = 0; k < matrix_inv->vesize; k++)
			for (vei_t i = 0; i < matrix_inv->vesize; i++)
				matrix_inv->vector[k].set_comp(-matrix_inv->vector[k].get_comp(i),F64B_TYPE, i);				
		return *matrix_inv;
	}
	else 
	{
		delete matrix_inv;
		return detinv(*this);
	}
}

oas_t DVectorMatrix::cofactor(DVectorMatrix& mat, vei_t i, vei_t j)
{
 	DVectorMatrix* ma;
	vei_t ho=0;
	vei_t ve=0;
	ma = new DVectorMatrix((mat.hosize-1), (mat.vesize-1));
	for (vei_t k=0; k<(mat.vesize-1); k++)
	{
		if (ve==i)
			ve++;
			
		for (vei_t l=0; l<(mat.hosize-1); l++)
		{
			if (ho==j)
				ho++;
				
			ma->vector[k].set_comp(mat.vector[ve].get_comp(ho), F64B_TYPE,l);
			ho++; 
		}	
		ho = 0;
		ve++;
	}
	return (pow(-1.0,(i+j))*ma->det());
}
 
DVectorMatrix DVectorMatrix::detinv(DVectorMatrix& mat)
{
	DVectorMatrix *matrix_inv;
	matrix_inv = new  DVectorMatrix(hosize, vesize);
	
 	oas_t dete = mat.det();
 	for (vei_t k=0; k<mat.vesize; k++)
	{
		for (vei_t l=0; l<mat.hosize; l++)
		{
 			matrix_inv->vector[k].set_comp((1/dete)*cofactor(mat,l,k), F64B_TYPE,l);
		}
	}
	return *matrix_inv;
}
  
 

oas_t  DVectorMatrix::det()
{
 	oas_t val=0.0;
	vei_t direc;
	if (hosize == vesize)
	{
		vei_t sum = (hosize + vesize);
		oas_t comp[sum];
		for (vei_t l=0; l<sum; l++)
			comp[l] = 1.0;
		for (vei_t i=0; i<(vesize); i++)
     		{
			for (vei_t j=0; j<(hosize); j++)
			{
				direc = i-j;
				if (direc < 0)
					direc = abs(direc);
				else if (direc > 0)
					direc = (hosize - direc);
						
				comp[direc] *= (vector[i].get_comp(j));
				direc = i+j+1;	
				if (direc < hosize)
					direc = hosize + direc;
					
				comp[direc] *= (vector[i].get_comp(j));
			}  
     		}
		for (vei_t k=0; k<(sum); k++)
		{
			if (k<hosize)
				val += comp[k];
			else
				val -= comp[k];
		}
		return val;
	}
	else
		return 0.0;
}

DVectorMatrix& DVectorMatrix::operator=(const DVectorMatrix& input) 
{
	hosize = input.hosize;
	vesize = input.vesize;
	
	vector = new DVector[vesize];
		
	for (vei_t i=0; i<vesize; i++)
		vector[i] = input.vector[i];
			
	return *this;
}

DVectorMatrix& operator*(const DVectorMatrix& mat1, const DVectorMatrix& mat2_T)
{
 	DVectorMatrix *mat;
 	mat = new DVectorMatrix(mat2_T.vesize, mat1.vesize);
	oas_t sum = 0.0;
	
	if (mat1.vesize==mat2_T.vesize)
	{
		for (vei_t i=0; i<mat1.vesize; i++)
		{	
			for (vei_t j=0; j<mat2_T.vesize; j++)
			{
				
				sum += mat1.vector[i] * mat2_T.vector[j];
				
				mat->vector[i].set_comp(sum, F64B_TYPE, j);
				sum = 0.0;
			}
		}
	}
		
	return *mat;
}

DVector& operator*(const DVector& vec, const DVectorMatrix& mat_T)
{
 	DVector *vect;
 	vect = new DVector(mat_T.vesize);

	for (vei_t j=0; j<mat_T.vesize; j++)	
		vect->set_comp(vec * mat_T.vector[j], F64B_TYPE, j);

	return *vect;
} 

DVector& operator*(const DVectorMatrix& mat, const DVector& vec)
{
 	DVector *vect;
 	vect = new DVector(mat.vesize);

	for (vei_t j=0; j<mat.vesize; j++)	
		vect->set_comp(vec * mat.vector[j], F64B_TYPE, j);

	return *vect;
} 

DVectorMatrix& operator+(const DVectorMatrix& mat1, const DVectorMatrix& mat2)
{
 	DVectorMatrix *mat;
	
	if ((mat1.hosize == mat2.hosize) && (mat1.vesize == mat2.vesize))
	{
		mat = new DVectorMatrix(mat2.vesize, mat1.vesize);
		for (vei_t i=0; i<mat1.vesize; i++)
			mat->vector[i] = mat1.vector[i] + mat2.vector[i];
	}
	else
		mat = NULL;
	
	return *mat;
}

DVectorMatrix& operator-(const DVectorMatrix& mat1, const DVectorMatrix& mat2)
{
 	DVectorMatrix *mat;
	
	if ((mat1.hosize == mat2.hosize) && (mat1.vesize == mat2.vesize))
	{
		mat = new DVectorMatrix(mat2.vesize, mat1.vesize);
		for (vei_t i=0; i<mat1.vesize; i++)
			mat->vector[i] = mat1.vector[i] - mat2.vector[i];
	}
	else
		mat = NULL;
	
	return *mat;
}

/********************************************************************************************************
* 		List of DVectorMatrices with pointer to next element in list, 				*
*		a DVector element associated with each Matrix and a integer variable			*
*		for class informations									*
********************************************************************************************************/

DMatrixList::DMatrixList() 
{
	matrix=NULL;
	next=NULL;
	mue=NULL;
}

DMatrixList::DMatrixList(vei_t hsize, vei_t vsize) 
{
	matrix = new DVectorMatrix(hsize, vsize); 
	mue = new DVector(hsize);
}
		
DMatrixList::~DMatrixList() 
{
	if (matrix) delete matrix;
	if (mue) delete mue;
}
		
void DMatrixList::create(vei_t hsize, vei_t vsize) 
{
	matrix = new DVectorMatrix(hsize, vsize); 
	mue = new DVector(hsize);
}







