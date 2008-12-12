/***************************************************************************
                           kmeans_t.cpp  -  v.1.00
                           -----------------------
    begin                : Mon Nov 17 2003
    copyright            : (C) 2003 by Martin Berchtold, Dikaios Papadogkonas
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

#include <stdlib.h> // exit()

#include "cstk_base/types.h"
#include "cstk_base/vector/dvector.h"
#include "algorithms/cmeans/cmeans.h"

int main(int ac, char *argv[]) {

	if (ac<2) {
		printf("\n  This demo will show how K-Means Clustering works\n");
		printf("  The number of Clusters (k), the amount of \n");
		printf("  training vectors and the fuzzy value can be adjusted.\n");
		printf("  The values that are going to be used are of the DVector\n");
		printf("  type ((1)U8B_TYPE, (2)S32B_TYPE, (3)F32B_TYPE and\n");
		printf("  (4)F64B_TYPE) with random elements.\n");
		printf("\n   syntax:");
		printf("\n     %s <k> <nu> <al> <dis> <ex>", argv[0]);
		printf("\n");
		printf("\n    <k> is the amount of buckets.");   
		printf("\n    <nu> is the amount of training vectors.");
		printf("\n    <al> stands for'm'.");
		printf("\n    <dis> selects the distance to be used.");
		printf("\n          Distances are: 2=DIS_EUCL (default)");
		printf("\n                         0=DIS_MANH");
		printf("\n                         3=DIS_MINK");
		printf("\n                         1=DIS_CHEB");
		printf("\n    <ex> specifies the exponent (default 2) of Minkowski.\n");
		printf("\n    try for instance '%s 3 50 0.5'.", argv[0]);
		printf("\n\n");
		exit(0);
	}
	
	vei_t k = atoi(argv[1]);
	vei_t nu = atoi(argv[2]);
	f_64b alp = atof(argv[3]);
	vei_t maxIter = 40;
	vei_t exp = 2;
	ve_t dis = 2;
	if (ac>4)
		dis= atoi(argv[4]);
	if (ac>5)
 		exp = atoi(argv[5]);
	
	DVector v(4), w(4);
	CMeans cluster(k, alp, dis, exp, maxIter);
	
		
	printf("\n(2)  training the bucket-vectors with %i random vectors\n\n",nu);
	for (vei_t i=0; i<nu; i++)
	{
		w.set_comp(((u_8b_max)*((double)(rand())/(double)(RAND_MAX+1))), U8B_TYPE, 0);
		w.set_comp(((s_32b_min)*((double)(rand())/(double)(RAND_MAX+1))),  S32B_TYPE,1);
		w.set_comp(((f_32b_min/1)*((double)(rand())/(double)(RAND_MAX+1))), F32B_TYPE,2);
			
		if (floor(2.0*rand()/(RAND_MAX+1.0))==1)	
			w.set_comp(((f_64b_max/2)*((double)(rand())/(double)(RAND_MAX+1))),  F64B_TYPE,3); 
		else
			w.set_comp(((f_64b_min/2)*((double)(rand())/(double)(RAND_MAX)*4)),  F64B_TYPE,3);
		printf("\t%s\n",w.to_string());
		cluster.read_vec(w);
	} 
	
	cluster.run();
	/*printf("\n(3)  inserting the last vector and retrieving his cluster\n\r");
	w.set_comp(((u_8b_max)*((double)(rand())/(double)(RAND_MAX+1))), U8B_TYPE, 0);
	w.set_comp(((s_32b_min)*((double)(rand())/(double)(RAND_MAX+1))),  S32B_TYPE,1);
	w.set_comp(((f_32b_min/1)*((double)(rand())/(double)(RAND_MAX+1))), F32B_TYPE,2);
	w.set_comp(((f_64b_min/2)*((double)(rand())/(double)(RAND_MAX)+1)),  F64B_TYPE,3);
		
	printf("\n\t%s\n",w.to_string());
	printf("\t\t-> the vector is closest to: %i\n\n", cluster.retrieve_cluster(w));
	printf("\tthe final bucket vectors are:\n\n");*/
	for (vei_t h=0; h<k; h++)
		printf("\t%s\n",cluster.clusterCenters[h].to_string());
	printf("\n\n");
	
	return 0;
}
