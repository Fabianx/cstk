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
 #include "cstk_base/matrix/dvectormatrix.h"
 
  void printv(DVector& v) 
 {
 	
   	for (vei_t i=0;i<v.get_dim();i++) 
   	{
       		switch (v.get_type(i)) 
		{
        		case U8B_TYPE:  printf(" %3.0f",v.get_comp(i));
					break;
        		case U16B_TYPE:	printf(" %10.0f",v.get_comp(i));
					break;
        		case U32B_TYPE: printf(" %15.0f",v.get_comp(i));
					break;
        		case U64B_TYPE: printf(" %20.0f",v.get_comp(i));
					break;
        		case S8B_TYPE:  printf(" %4.0f",v.get_comp(i));
					break;
        		case S16B_TYPE: printf(" %10.0f",v.get_comp(i));
					break;
        		case S32B_TYPE: printf(" %15.0f",v.get_comp(i)); 
					break;  
        		case S64B_TYPE: printf(" %20.0f",v.get_comp(i));
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
 	DVectorMatrix A(4,4), B(3,4), C(4,3), D;
	DVector a(4), b(3), c;
	oas_t x;
	
	printf("---------------------------------------------------\n\r"); 
	printf("Test prgram for DVectorMatrix implementation, which\n\r");
	printf("uses an array of DVector. The implemented operators\n\r");
	printf("and methods to manipulate matrices are tested. The\n\r");
	printf("operators are =, +, - and *. The methods are trans-\n\r");
	printf("pose, invert and determinant.\n\r");
	printf("---------------------------------------------------\n\r"); 
	
	printf("\n\r");
	printf("Matrix A(4X4):\n\r");
	A.vector[0].set_comp(167, U8B_TYPE,0); 
	A.vector[0].set_comp(293747, U16B_TYPE,1); 
	A.vector[0].set_comp(498098783, U32B_TYPE,2); 
	A.vector[0].set_comp(120, S8B_TYPE,3);
	printv(A.vector[0]);
	A.vector[1].set_comp(22, U8B_TYPE,0); 
	A.vector[1].set_comp(1298, U16B_TYPE,1); 
	A.vector[1].set_comp(123098043, U32B_TYPE,2); 
	A.vector[1].set_comp(-23, S8B_TYPE,3);
	printv(A.vector[1]);
	A.vector[2].set_comp(123, U8B_TYPE,0); 
	A.vector[2].set_comp(4222, U16B_TYPE,1); 
	A.vector[2].set_comp(51432577, U32B_TYPE,2); 
	A.vector[2].set_comp(33, S8B_TYPE,3);
	printv(A.vector[2]);
	A.vector[3].set_comp(150, U8B_TYPE,0); 
	A.vector[3].set_comp(4356, U16B_TYPE,1); 
	A.vector[3].set_comp(449803, U32B_TYPE,2); 
	A.vector[3].set_comp(-112, S8B_TYPE,3);
	printv(A.vector[3]);
	
	printf("Matrix B(4X3):\n\r");
	B.vector[0].set_comp(37, U8B_TYPE,0); 
	B.vector[0].set_comp(56747, U16B_TYPE,1); 
	B.vector[0].set_comp(449098783, U32B_TYPE,2); 
	printv(B.vector[0]);
	B.vector[1].set_comp(22, U8B_TYPE,0); 
	B.vector[1].set_comp(128, U16B_TYPE,1); 
	B.vector[1].set_comp(123309043, U32B_TYPE,2); 
	printv(B.vector[1]);
	B.vector[2].set_comp(13, U8B_TYPE,0); 
	B.vector[2].set_comp(4223, U16B_TYPE,1); 
	B.vector[2].set_comp(61472577, U32B_TYPE,2); 
	printv(B.vector[2]);
	B.vector[3].set_comp(178, U8B_TYPE,0); 
	B.vector[3].set_comp(4363, U16B_TYPE,1); 
	B.vector[3].set_comp(44909503, U32B_TYPE,2); 
	printv(B.vector[3]);
	
	printf("Matrix C(3X4):\n\r");
	C.vector[0].set_comp(167, U8B_TYPE,0); 
	C.vector[0].set_comp(293747, U16B_TYPE,1); 
	C.vector[0].set_comp(498098783, U32B_TYPE,2); 
	C.vector[0].set_comp(120, S8B_TYPE,3);
	printv(C.vector[0]);
	C.vector[1].set_comp(22, U8B_TYPE,0); 
	C.vector[1].set_comp(1298, U16B_TYPE,1); 
	C.vector[1].set_comp(123098043, U32B_TYPE,2); 
	C.vector[1].set_comp(-23, S8B_TYPE,3);
	printv(C.vector[1]);
	C.vector[2].set_comp(123, U8B_TYPE,0); 
	C.vector[2].set_comp(4222, U16B_TYPE,1); 
	C.vector[2].set_comp(51432577, U32B_TYPE,2); 
	C.vector[2].set_comp(33, S8B_TYPE,3);
	printv(C.vector[2]);
	printf("\n\r");
	
	printf("Vector a:\n\r");
	a.set_comp(22, U8B_TYPE,0); 
	a.set_comp(128, U16B_TYPE,1); 
	a.set_comp(123309043, U32B_TYPE,2); 
	a.set_comp(37, U8B_TYPE,3); 
	printv(a);
	printf("Vector b:\n\r");
	b.set_comp(5, U8B_TYPE,0); 
	b.set_comp(1706232434, U64B_TYPE,1); 
	b.set_comp(-3309043, S32B_TYPE,2); 
	printv(b);
	printf("\n\r");
	
	printf("Addition: D = A + A\n");
	D = A + A;
	for (vei_t i=0; i<4; i++)
		printv(D.vector[i]);
		 
	printf("Subtraction: D = A - A\n");
	D = A - A;
	for (vei_t i=0; i<4; i++)
		printv(D.vector[i]);
		
	printf("Multiplication and transposition: D = B * C_T\n");
	D = B * C.T();
	for (vei_t i=0; i<4; i++)
		printv(D.vector[i]);
	
	printf("Determinant of matrix A: %7.3e\n",A.det());	
	
	printf("Inversion and multiplication: D = A_-1 * A_T\n");
	D = A.INV() * A.T();
	for (vei_t i=0; i<4; i++)
		printv(D.vector[i]);
		
	printf("Multiplication with vectors: c = a * B_T\n");
	c = (a * B.T());// * b);
	printv(c);
	x = c * b;
	printf("Multiplication of vectors: x = c * b = %7.3e\n",x);
		 
	return 0; 
 }
 
 
 
 
 
 
 
 
 
 