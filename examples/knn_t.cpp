/***************************************************************************
                            knn_t.cpp  -  v.1.00
                           ----------------------
    begin                : Mon Nov 17 2003
    copyright            : (C) 2003 by Martin Berchtold, Dikaios Papadogkonas
    email                :  
 ***************************************************************************/

#include <stdlib.h>

#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h"
#include "algorithms/knn/knn.h"

#define k 3
#define vecnum 10
#define size 4
#define dis DIS_EUCL

int main()
	{
	KNN nn(k);
	
	VectorPoCl *v;
	v = new VectorPoCl[vecnum];
	
	printf("---------------------------------------------------\n\r"); 
	printf("Test prgram for K Nearest Neighbour (KNN) algorithm\n\r");
	printf("  with K=%i and %i vectors with %i elements\n\r",k,vecnum,size);
	printf("---------------------------------------------------\n\r"); 
	
	for (vei_t j=0; j<vecnum; j++)
		v[j].create(size);
		
	v[0].vector->set_comp(136,     U8B_TYPE, 0); 
	v[0].vector->set_comp(199481,  S32B_TYPE,1); 
	v[0].vector->set_comp(14.2914, F32B_TYPE,2); 
	v[0].vector->set_comp(-16122,  F64B_TYPE,3);
	v[0].vec_class = 2;
	
	v[1].vector->set_comp(57,     U8B_TYPE, 0); 
	v[1].vector->set_comp(152313,  S32B_TYPE,1); 
	v[1].vector->set_comp(6.3323, F32B_TYPE,2); 
	v[1].vector->set_comp(-4233,  F64B_TYPE,3);
	v[1].vec_class = 1;
	
	v[2].vector->set_comp(136,     U8B_TYPE, 0); 
	v[2].vector->set_comp(123456,  S32B_TYPE,1); 
	v[2].vector->set_comp(4.2334, F32B_TYPE,2); 
	v[2].vector->set_comp(-5877,  F64B_TYPE,3);
	v[2].vec_class = 3;
	
	v[3].vector->set_comp(435,     U8B_TYPE, 0); 
	v[3].vector->set_comp(45345,  S32B_TYPE,1); 
	v[3].vector->set_comp(23.5895, F32B_TYPE,2); 
	v[3].vector->set_comp(-3422,  F64B_TYPE,3);
	v[3].vec_class = 2;
	
	v[4].vector->set_comp(323,     U8B_TYPE, 0); 
	v[4].vector->set_comp(23880,  S32B_TYPE,1); 
	v[4].vector->set_comp(23.7545, F32B_TYPE,2); 
	v[4].vector->set_comp(-5680,  F64B_TYPE,3);
	v[4].vec_class = -1;
	
	v[5].vector->set_comp(875,     U8B_TYPE, 0); 
	v[5].vector->set_comp(86754,  S32B_TYPE,1); 
	v[5].vector->set_comp(22.4456, F32B_TYPE,2); 
	v[5].vector->set_comp(-9758,  F64B_TYPE,3);
	v[5].vec_class = -1;
	
	v[6].vector->set_comp(232,     U8B_TYPE, 0); 
	v[6].vector->set_comp(88754,  S32B_TYPE,1); 
	v[6].vector->set_comp(12.4345, F32B_TYPE,2); 
	v[6].vector->set_comp(-4900,  F64B_TYPE,3);
	v[6].vec_class = -1;
	
	v[7].vector->set_comp(874,     U8B_TYPE, 0); 
	v[7].vector->set_comp(507865,  S32B_TYPE,1); 
	v[7].vector->set_comp(11.3467, F32B_TYPE,2); 
	v[7].vector->set_comp(-1658,  F64B_TYPE,3);
	v[7].vec_class = -1;
	
	v[8].vector->set_comp(175,     U8B_TYPE, 0); 
	v[8].vector->set_comp(86554,  S32B_TYPE,1); 
	v[8].vector->set_comp(26.4456, F32B_TYPE,2); 
	v[8].vector->set_comp(-4558,  F64B_TYPE,3);
	v[8].vec_class = -1;
	
	v[9].vector->set_comp(136,     U8B_TYPE, 0); 
	v[9].vector->set_comp(167481,  S32B_TYPE,1); 
	v[9].vector->set_comp(12.9490, F32B_TYPE,2); 
	v[9].vector->set_comp(-12000,  F64B_TYPE,3);
	v[9].vec_class = -1;
	
	vei_t classes;
	for (vei_t i=0; i<vecnum; i++)
	{
		classes = nn.access(v[i], dis);
		if (classes!=-1) {
			printf("for vector %i:\n\t%s ",i, v[i].vector->to_string());
			printf("\nthe estimated class is %i.\n\r",classes);
		}
		else
			printf("written vector %i:\n\t%s\n",i, v[i].vector->to_string());
	}
	
	printf("---------------------------------------------------\n\r");
	
	return 0;
	}
