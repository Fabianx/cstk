/***************************************************************************
                           stochastics_t.cpp  -  v.1.00
                             -------------------
    begin                : Mo Oct 04 2004
    copyright            : (C) 2002-2003 by Martin Berchtold
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

 #include "cstk_base/vector/dvector.h"
 #include "cstk_base/stochastics/covariance/covariance.h"
 #include "cstk_base/stochastics/mvg/mvg.h"
 #include "cstk_base/matrix/dvectormatrix.h"
 
  void printv3(DVector& v) 
 {
 	
   	for (vei_t i=0;i<v.get_dim();i++) 
   	{
       		switch (v.get_type(i)) 
		{
        		case U8B_TYPE:  printf(" %3.0f",v.get_comp(i));
					break;
        		case U16B_TYPE:	printf(" %3.0f",v.get_comp(i));
					break;
        		case U32B_TYPE: printf(" %d",v.get_comp(i));
					break;
        		case U64B_TYPE: printf(" %d",v.get_comp(i));
					break;
        		case S8B_TYPE:  printf(" %d",v.get_comp(i));
					break;
        		case S16B_TYPE: printf(" %10.0f",v.get_comp(i));
					break;
        		case S32B_TYPE: printf(" %10.0f",v.get_comp(i)); 
					break;  
        		case S64B_TYPE: printf(" %10.0f",v.get_comp(i));
					break;
        		case F32B_TYPE: printf(" %7.3e",v.get_comp(i));
					break;
        		case F64B_TYPE: printf(" %7.3e",v.get_comp(i)); 
					break;
     		}
     	}
	printf("\n\r");
 }
 
 int main() 
 {
 	Covariance covm;
	MVG gauss;
	
	DVectorList *v,*w,*x;
	v = new DVectorList[5];
	w = new DVectorList[5];
	x = new DVectorList[5];

	DMatrixList *ml;
	ml = new DMatrixList[3];
	
	DVectorMatrix m(4,4);
	for (vei_t j=0; j<5; j++)
		v[j].create(4);
	
	v[0].vector->set_comp(11, U8B_TYPE,0); 
	v[0].vector->set_comp(1511, U16B_TYPE,1); 
	v[0].vector->set_comp(35, U8B_TYPE,2); 
	v[0].vector->set_comp(48, U8B_TYPE,3);
	v[0].next = &v[1];
	printv3(*(v[0].vector));
	v[1].vector->set_comp(5, U8B_TYPE,0); 
	v[1].vector->set_comp(20, U8B_TYPE,1); 
	v[1].vector->set_comp(7, U8B_TYPE,2); 
	v[1].vector->set_comp(8, U8B_TYPE,3);
	v[1].next = &v[2];
	printv3(*(v[1].vector));
	v[2].vector->set_comp(9123, U16B_TYPE,0); 
	v[2].vector->set_comp(56, U8B_TYPE,1); 
	v[2].vector->set_comp(111, U8B_TYPE,2); 
	v[2].vector->set_comp(121, U8B_TYPE,3);
	v[2].next = &v[3];
	printv3(*(v[2].vector));
	v[3].vector->set_comp(131, U8B_TYPE,0); 
	v[3].vector->set_comp(370, U16B_TYPE,1); 
	v[3].vector->set_comp(56, U8B_TYPE,2); 
	v[3].vector->set_comp(161, U8B_TYPE,3);
	v[3].next = &v[4];
	printv3(*(v[3].vector));
	v[4].vector->set_comp(1551, U16B_TYPE,0); 
	v[4].vector->set_comp(127, U8B_TYPE,1); 
	v[4].vector->set_comp(150, U8B_TYPE,2); 
	v[4].vector->set_comp(205, U8B_TYPE,3);
	v[4].next = NULL;
	printv3(*(v[4].vector));
	printf("\n\r");
	
	for (vei_t j=0; j<5; j++)
		w[j].create(4);
	
	w[0].vector->set_comp(111, U8B_TYPE,0); 
	w[0].vector->set_comp(3391, U16B_TYPE,1); 
	w[0].vector->set_comp(34, U8B_TYPE,2); 
	w[0].vector->set_comp(48, U8B_TYPE,3);
	w[0].next = &w[1];
	printv3(*(w[0].vector));
	w[1].vector->set_comp(52, U8B_TYPE,0); 
	w[1].vector->set_comp(103, U8B_TYPE,1); 
	w[1].vector->set_comp(7, U8B_TYPE,2); 
	w[1].vector->set_comp(82, U8B_TYPE,3);
	w[1].next = &w[2];
	printv3(*(w[1].vector));
	w[2].vector->set_comp(923, U16B_TYPE,0); 
	w[2].vector->set_comp(51, U8B_TYPE,1); 
	w[2].vector->set_comp(201, U8B_TYPE,2); 
	w[2].vector->set_comp(11, U8B_TYPE,3);
	w[2].next = &w[3];
	printv3(*(w[2].vector));
	w[3].vector->set_comp(101, U8B_TYPE,0); 
	w[3].vector->set_comp(700, U16B_TYPE,1); 
	w[3].vector->set_comp(56, U8B_TYPE,2); 
	w[3].vector->set_comp(61, U8B_TYPE,3);
	w[3].next = &w[4];
	printv3(*(w[3].vector));
	w[4].vector->set_comp(5111, U16B_TYPE,0); 
	w[4].vector->set_comp(17, U8B_TYPE,1); 
	w[4].vector->set_comp(155, U8B_TYPE,2); 
	w[4].vector->set_comp(5, U8B_TYPE,3);
	w[4].next = NULL;
	printv3(*(w[4].vector));
	printf("\n\r");
	
	for (vei_t j=0; j<5; j++)
		x[j].create(4);
	
	x[0].vector->set_comp(121, U8B_TYPE,0); 
	x[0].vector->set_comp(3021, U16B_TYPE,1); 
	x[0].vector->set_comp(4, U8B_TYPE,2); 
	x[0].vector->set_comp(48, U8B_TYPE,3);
	x[0].next = &x[1];
	printv3(*(x[0].vector));
	x[1].vector->set_comp(521, U16B_TYPE,0); 
	x[1].vector->set_comp(13, U8B_TYPE,1); 
	x[1].vector->set_comp(7, U8B_TYPE,2); 
	x[1].vector->set_comp(2, U8B_TYPE,3);
	x[1].next = &x[2];
	printv3(*(x[1].vector));
	x[2].vector->set_comp(9230, U16B_TYPE,0); 
	x[2].vector->set_comp(51, U8B_TYPE,1); 
	x[2].vector->set_comp(240, U8B_TYPE,2); 
	x[2].vector->set_comp(11, U8B_TYPE,3);
	x[2].next = &x[3];
	printv3(*(x[2].vector));
	x[3].vector->set_comp(112, U8B_TYPE,0); 
	x[3].vector->set_comp(70, U16B_TYPE,1); 
	x[3].vector->set_comp(56, U8B_TYPE,2); 
	x[3].vector->set_comp(67, U8B_TYPE,3);
	x[3].next = &x[4];
	printv3(*(x[3].vector));
	x[4].vector->set_comp(511, U16B_TYPE,0); 
	x[4].vector->set_comp(72, U8B_TYPE,1); 
	x[4].vector->set_comp(15, U8B_TYPE,2); 
	x[4].vector->set_comp(52, U8B_TYPE,3);
	x[4].next = NULL;
	printv3(*(x[4].vector));
	printf("\n\r");
	
	printf("The first plus the second vector is:\n\r");
	DVector s1, s2, s3;
	s1 = (*(v[0].vector));
	s2 = (*(v[1].vector));
	DVector s0(*(v[0].vector));
	printv3(s0);
	printv3(s1);
	printv3(s2);
	printf("\n");
	s3 = s1 + s2;
	printv3(s3);
	printf("The second minus the first vector is:\n\r");
	s3 = s2 -s1;
	printv3(s3);
	
	printf("The covariance matrix of these vectors is:\n\r");
	DVectorMatrix cov1(covm.matrix(v[0]));
	ml[0].mue = new DVector(*(covm.mean));
	ml[0].classinf = 1;
	//ml[0].next = &ml[1];
	printv3(*(covm.mean));
	printf("\n");
	for (vei_t i=0; i<4; i++)
		printv3(cov1.vector[i]);
	printf("\n\rdeterminant: %7.1e\n",cov1.det());
	printf("\n");
	
	printf("The covariance matrix of these vectors is:\n\r");
	DVectorMatrix cov2(covm.matrix(w[0]));
	ml[1].mue = new DVector(*(covm.mean));
	ml[1].classinf = 2;
	//ml[1].next = &ml[3];
	printv3(*(covm.mean));
	printf("\n");
	for (vei_t i=0; i<4; i++)
		printv3(cov2.vector[i]);
	printf("\n\rdeterminant: %7.1e\n",cov2.det());
	printf("\n");
		
	printf("The covariance matrix of these vectors is:\n\r");
	DVectorMatrix cov3(covm.matrix(x[0]));
	ml[2].mue = new DVector(*(covm.mean));
	ml[2].classinf = 3;
	//ml[2].next = NULL;
	printv3(*(covm.mean));
	printf("\n");
	for (vei_t i=0; i<4; i++)
		printv3(cov3.vector[i]);
	printf("\n\rdeterminant: %7.1e\n",cov3.det());
	printf("\n");
	
	printf("The inversion of the first covariance matrix is:\n\r");
	ml[0].matrix = new DVectorMatrix(cov1.INV());
	ml[0].next = &ml[1];
	for (vei_t i=0; i<ml[0].matrix->vesize; i++)
		printv3(ml[0].matrix->vector[i]);
	printf("\n");
	DVectorMatrix A1((*(ml[0].matrix)) * cov1.T());
	for (vei_t i=0; i<ml[0].matrix->vesize; i++)
		printv3(A1.vector[i]);
		
	printf("The inversion of the second covariance matrix is:\n\r");
	ml[1].matrix = new DVectorMatrix(cov2.INV());
	ml[1].next = &ml[2];
	for (vei_t i=0; i<ml[1].matrix->vesize; i++)
		printv3(ml[1].matrix->vector[i]);
	printf("\n");
	DVectorMatrix A2((*(ml[1].matrix)) * cov2.T());
	for (vei_t i=0; i<ml[1].matrix->vesize; i++)
		printv3(A2.vector[i]);
		
	printf("The inversion of the second covariance matrix is:\n\r");
	ml[2].matrix = new DVectorMatrix(cov3.INV());
	ml[2].next = NULL;
	for (vei_t i=0; i<ml[2].matrix->vesize; i++)
		printv3(ml[2].matrix->vector[i]);
	printf("\n");
	DVectorMatrix A3((*(ml[2].matrix)) * cov3.T());
	for (vei_t i=0; i<ml[2].matrix->vesize; i++)
		printv3(A3.vector[i]);
		
	printf("\n\r");
	printf("Given Matrix:\n\r");
	m.vector[0].set_comp(1, U8B_TYPE,0); 
	m.vector[0].set_comp(2, U8B_TYPE,1); 
	m.vector[0].set_comp(3, U8B_TYPE,2); 
	m.vector[0].set_comp(3, U8B_TYPE,3);
	printv3(m.vector[0]);
	m.vector[1].set_comp(2, U8B_TYPE,0); 
	m.vector[1].set_comp(1, U8B_TYPE,1); 
	m.vector[1].set_comp(1, U8B_TYPE,2); 
	m.vector[1].set_comp(5, U8B_TYPE,3);
	printv3(m.vector[1]);
	m.vector[2].set_comp(1, U8B_TYPE,0); 
	m.vector[2].set_comp(4, U8B_TYPE,1); 
	m.vector[2].set_comp(1, U8B_TYPE,2); 
	m.vector[2].set_comp(3, U8B_TYPE,3);
	printv3(m.vector[2]);
	m.vector[3].set_comp(1, U8B_TYPE,0); 
	m.vector[3].set_comp(4, U8B_TYPE,1); 
	m.vector[3].set_comp(4, U8B_TYPE,2); 
	m.vector[3].set_comp(1, U8B_TYPE,3);
	printv3(m.vector[3]);
	printf("\n\r");
		
	printf("The determinant of the matrix is: %7.3e\n\r",m.det());
		
	printf("The transposition of the matrix:\n\r");
	for (vei_t i=0; i<m.hosize; i++)
		printv3(m.T().vector[i]);
		
	printf("The inversion of the matrix:\n\r");
	for (vei_t i=0; i<m.vesize; i++)
		printv3(m.INV().vector[i]);
		
	printf(" det=%7.1e\n\r",m.INV().det());
	printf("\n\r");
		
	printf("The multiplication of the matrix with it's inversion:\n\r");
	
	DVectorMatrix m_r, m_k;
	DVector m_s;
	m_r = m.INV() * m.T();
	for (vei_t i=0; i<m_r.vesize; i++)
		printv3(m_r.vector[i]);
	printf("\n");
	m_s = (*(v[0].vector)) * m.T();
	printv3(m_s);
	printf("\n");
	m_s = m * (*(v[0].vector));
	printv3(m_s);
	printf("\n");
	m_k = m;
	for (vei_t i=0; i<m_r.vesize; i++)
		printv3(m_k.vector[i]);
	printf("\n");
	for (vei_t i=0; i<m_r.vesize; i++)
		printv3(m_r.vector[i]);
		
	printf("The multivariant gaussians scan delivers class: %i\n\r",gauss.value(ml[0],(*(x[2].vector))));
		
 	return 0;
 }
 
 
 