/***************************************************************************
                            knn_t.cpp  -  v.1.00
                           ----------------------
    begin                : Mon Nov 17 2003
    copyright            : (C) 2003 by Martin Berchtold, Dikaios Papadogkonas,
                           Kristof Van Laerhoven
    email                :  
 ***************************************************************************/

#include <stdlib.h>

#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h"
#include "algorithms/knn/knn.h"

#define k 3
#define vecnum 4
#define size 4
#define dis DIS_EUCL

void test(KNN *nn, VectorPoCl *v) {
	vei_t ret = nn->access(*v, dis);
	if (ret!=-1) {
			printf("for vector:\t[%s] ...",v->vector->to_string());
			printf("\n the estimated class is %i",ret);
			printf(" with confidence %g.\n", nn->get_conf());
			printf(" k classes: %i, %i, %i.\n", nn->get_class(0),
			nn->get_class(1), nn->get_class(2));
			printf(" k distances: %g, %g, %g.\n", nn->get_dist(0),
			nn->get_dist(1), nn->get_dist(2));
	}
	else
			printf("stored vector:\t[%s] -> %i\n",v->vector->to_string(), 
			       v->vec_class );
}

int main()
{
	KNN nn(k);

	VectorPoCl v(size);

	printf("---------------------------------------------------\n\r"); 
	printf("Test prgram for K Nearest Neighbour (KNN) algorithm\n\r");
	printf("  with K=%i and %i vectors with %i elements\n\r",k,vecnum,size);
	printf("---------------------------------------------------\n\r"); 

	v.vector->set_comp(136,     U8B_TYPE, 0); 
	v.vector->set_comp(199481,  S32B_TYPE,1); 
	v.vector->set_comp(14.2914, F32B_TYPE,2); 
	v.vector->set_comp(-16122,  F64B_TYPE,3);
	v.vec_class = 1;
	test( &nn, &v);

	v.vector->set_comp(57,     U8B_TYPE, 0); 
	v.vector->set_comp(133466,  S32B_TYPE,1); 
	v.vector->set_comp(6.3323, F32B_TYPE,2); 
	v.vector->set_comp(-4233,  F64B_TYPE,3);
	v.vec_class = 2;
	test( &nn, &v);

	v.vector->set_comp(49,     U8B_TYPE, 0); 
	v.vector->set_comp(152119,  S32B_TYPE,1); 
	v.vector->set_comp(2.4961, F32B_TYPE,2); 
	v.vector->set_comp(-5123,  F64B_TYPE,3);
	v.vec_class = 2;
	test( &nn, &v);

	v.vector->set_comp(146,     U8B_TYPE, 0); 
	v.vector->set_comp(133456,  S32B_TYPE,1); 
	v.vector->set_comp(4.2334, F32B_TYPE,2); 
	v.vector->set_comp(-5877,  F64B_TYPE,3);
	v.vec_class = 3;
	test( &nn, &v);

	v.vector->set_comp(136,     U8B_TYPE, 0); 
	v.vector->set_comp(123456,  S32B_TYPE,1); 
	v.vector->set_comp(4.2334, F32B_TYPE,2); 
	v.vector->set_comp(-5877,  F64B_TYPE,3);
	v.vec_class = -1;
	test( &nn, &v);

	printf("---------------------------------------------------\n\r");
	
	return 0;
}
