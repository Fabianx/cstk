/***************************************************************************
                            sdma_t.cpp  -  sdm.av.1.00
                             -------------------
    begin                : Sep 09 2004
    copyright            : (C) 2004 by Martin Berchtold
    email                : berch@teco.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

 /**Example using SDMA for Creating and Maintaining a Sparse Distributed Memory Approached
  *@author Martin Berchtold

   This demo shows what the sdma can do - most of the explanations
   get printed out, so just run it and read.
 */

 #include <time.h>
 
 #include "algorithms/sdm/sdmafct.h"
 #include "cstk_base/vector/kvector.h"
 
 #define DENS 0.3
 #define DENS_INIT 0.3
 
 void printv(BinVector *v) 
 {
   for (vei_t i=0;i<v->get_dim();i++) 
       printf("%i",v->get_comp(i));
   printf("\n\r");       
 }

 void printv2(BVector<s_16b> *v) 
 {
   for (vei_t i=0;i<v->get_dim();i++) 
       printf(" %3i",v->get_comp(i));
 }
 
 void printv3(BVector<oas_t> *v) 
 {
   for (vei_t i=0;i<v->get_dim();i++) 
       printf(" %f",v->get_comp(i));
 }
  
 int main(int ac, char *argv[]) {

	if (ac<2) {
		printf("\n  This demo will show how a Sparse Distributed Memory\n");
		printf("  works. The implementation uses the advanced implementation \n");
		printf("  with different distributions.\n");
		printf("  The distributions are relativ and absolute, with linear\n");
		printf("  and gaussian functions (argument's are (1) linear,\n");
		printf("  (2) gaussian and (3) with NAND ditance masurement. After the\n");
		printf("  initialization phase rarely used entries are deleted.\n");
		printf("\n   syntax:");
		printf("\n     %s <df> <a/r> <al> <dl> <mems> <inits> <tests> <thrs> <rem>", argv[0]);
		printf("\n");
		printf("\n    <df> specifies the distribution function:");
		printf("\n		0=linear with Hamming distance,");
		printf("\n		1=gaussian with Hamming distance,");
		printf("\n		2=with NAND distance masurement.");   
		printf("\n    <a/r> absolute(0) or relativ(1) distance masurement.");
		printf("\n    <al> stands for the adress length.");
		printf("\n    <dl> stands for the data length.");
		printf("\n    <mems> specifies the number of address data vector tupels.");
		printf("\n    <inits> indicates the number of initialization vectors");
		printf("\n    <tests> indicates the number of vectors stored in the memory");
		printf("\n    <thres> specifies the threshold value indicates the percentage");
		printf("\n            that the influence goes down with each distance step.");
		printf("\n    <rem> indicates the minimum influence the initialization phase");
		printf("\n          must have had on each memory vector, so it doesn't gets deleted.\n"); 
		printf("\n    try for instance '%s 0 0 40 50 5000 500 6000 0.085 0.0',", argv[0]);
		printf("\n              or try '%s 1 0 40 50 2000 400 2400 0.075 0.0',", argv[0]);
		printf("\n              or try '%s 1 1 40 50 2000 500 2400 0.075 0.0',", argv[0]);
		printf("\n              or try '%s 2 1 40 50 2000 500 2400 0.07 0.0',", argv[0]);
		printf("\n              or try '%s 2 0 40 50 2000 500 2400 0.08 0.0'.", argv[0]);
		printf("\n\n");
		exit(0);
	}
	
	vei_t ADDR_LEN=atoi(argv[3]);
	vei_t DATA_LEN=atoi(argv[4]);
	vei_t MEM_SIZE=atoi(argv[5]);
	vei_t INIT_SIZE=atoi(argv[6]);
	vei_t TEST_SIZE=atoi(argv[7]);
	oas_t THRS=atof(argv[8]);
	oas_t REM=atof(argv[9]);
	ve_t AbsRel=atoi(argv[2]);
	ve_t fct=atoi(argv[1]);
	
	SDMAfct sdma(MEM_SIZE, ADDR_LEN, DATA_LEN);
	sdma.create(MEM_SIZE, ADDR_LEN, DATA_LEN, THRS);
	BinVector v1, v2, b1, b2; 
	KVector hd(MEM_SIZE); // keep all values in the vector
	vei_t d=0;
	
	v1.create(ADDR_LEN);  v2.create(ADDR_LEN);
	b1.create(DATA_LEN);  b2.create(DATA_LEN);
	for (vei_t i=0;i<ADDR_LEN;i++) { v1.set_comp(0,i);  v2.set_comp(0,i); }   
	for (vei_t i=0;i<DATA_LEN;i++) { b1.set_comp(0,i);  b2.set_comp(0,i); }
	v1.set_comp(1,2); v1.set_comp(1,11);  v1.set_comp(1,12); v1.tgl_comp(13); 
	v1.tgl_comp(23); v1.tgl_comp(27);   
	b1.set_comp(1,2); b1.set_comp(1,12); b1.set_comp(1,22); b1.set_comp(1,17);
	
	printf(" Creating %i random vectors as the address space.\n\r", MEM_SIZE);  
	sdma.random_init();  
	
	printf(" Adding %i (random) vectors i for initialization  tempsum = new BVector<oas_t>;n random places.\n\r", INIT_SIZE); 
	int tm;
	tm = clock();
	for (vei_t k=0; k<INIT_SIZE; k++) 
	{  
     		for (vei_t i=0;i<ADDR_LEN;i++) 
           		 v2.set_comp(floor(2.0*rand()/(RAND_MAX+1.0)),i); 
		switch (fct)
		{
			case 0: d = sdma.store(v2, b2, AbsRel); break;
			case 1: d = sdma.store_gauss(v2, b2, AbsRel, DENS_INIT); break;
			case 2: d = sdma.store_nand(v2, b2, AbsRel); break;
		}
		if (k<10) 
		{
			printf(" %i: ",d);
			for (vei_t i=0;i<v2.get_dim();i++) 
			printf("%i",v2.get_comp(i));
			printf("  ");
			printv(&b2);
		} else printf(".");     
  	}
	tm = clock() - tm;
	printf("time during calculation: %i", tm);   
	printf("\n\r");
	
	printf("remove all empty addresses, address space is now %i entries large.", sdma.remove(REM));   
	printf("\n\r");
	
	printf(" Storing v1's data vector, which is:\n\r");
	printv(&b1);
	//sdma.store(v1, b1, AbsRel);
	switch (fct)
	{
		case 0: sdma.store(v1, b1, AbsRel); d = sdma.store(v1, b1, AbsRel); break;
		case 1: sdma.store_gauss(v1, b1, AbsRel, DENS_INIT); d = sdma.store_gauss(v1, b1, AbsRel, DENS); break;
		case 2: sdma.store_nand(v1, b1, AbsRel); d = sdma.store_nand(v1, b1, AbsRel); break;
	}
	printf(" in %i locations on address:\n\r",d);
	printv(&v1);
	
	printf(" Adding %i other (random) vectors i   tempsum = new BVector<oas_t>;n random places.\n\r", TEST_SIZE); 
	tm = clock();
	for (vei_t k=0; k<TEST_SIZE; k++) {  
		for (vei_t i=0;i<ADDR_LEN;i++) 
			v2.set_comp(floor(2.0*rand()/(RAND_MAX+1.0)),i); 
		for (vei_t i=0;i<DATA_LEN;i++) 
			b2.set_comp(floor(2.0*rand()/(RAND_MAX+1.0)),i); 
		switch (fct)
		{
			case 0: d = sdma.store(v2, b2, AbsRel); break;
			case 1: d = sdma.store_gauss(v2, b2, AbsRel, DENS); break;
			case 2: d = sdma.store_nand(v2, b2, AbsRel); break;
		}
		if (k<10) 
		{
			printf(" %i: ",d);
			for (vei_t i=0;i<v2.get_dim();i++) 
			printf("%i",v2.get_comp(i));
			printf("  ");
			printv(&b2);
		} else printf(".");     
	}
	tm = clock() - tm;
	printf("time during calculation: %i", tm);   
	printf("\n\r");
	
	printf(" Adding the winner's counters from retrieval of address.\n\r");
	BVector<s_16b> tsum2(b1.get_dim());
	BVector<oas_t> tsum3(b1.get_dim());
	BinVector sum;
	sum.create(DATA_LEN);
	switch (fct)
	{
		case 0: d = sdma.retrieve(v1,sum,tsum3,AbsRel); break;
		case 1: d = sdma.retrieve_gauss(v1,sum,tsum3,AbsRel,DENS); break;
		case 2: d = sdma.retrieve_nand(v1,sum,tsum3,AbsRel); break;
	}
	//sdm.retrieve(v1, tsum2, c);
	
	//printf("Which sums up to:\n\r");
	//printv2(&tsum2);  printf("\n\r"); 
	
	printf("Which sums up to:\n\r");
	printv3(&tsum3);  printf("\n\r");
	
	printf("And results in:\n\r");
	printv(&sum);
	
	printf("Which should match:\n\r");
	printv(&b1);
	
	printf("\n\rDone.\n\r");
	
	
	return 0;
}
