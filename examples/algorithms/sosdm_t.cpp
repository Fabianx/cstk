/***************************************************************************
                            sodm_t.cpp  -  sdm.av.1.00
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
 
 #include "algorithms/sdm/sosdm.h"
 #include "cstk_base/vector/kvector.h"
 #define REM 0.0
 
 void printv(BinVector *v) 
 {
   for (vei_t i=0;i<v->get_dim();i++) 
       printf("%i",v->get_comp(i));
   printf("\n\r");       
 }
 
 void printv3(BVector<oas_t> *v) 
 {
   for (vei_t i=0;i<v->get_dim();i++) 
       printf(" %f",v->get_comp(i));
 }
  
int main(int ac, char *argv[]) {

  if (ac<2) {
    printf("\n  This demo will show how a self organizing Sparse \n");
    printf("  Distributed Memory works. All training vectors and \n");
    printf("  input vectors are random. \n");
    printf("\n   syntax:");
    printf("\n     %s <addl> <datal> <memsize> <testsize> <fillsize> <thres> <initloops>", argv[0]);
    printf("\n");
    printf("\n    <addl> length of the address vectors,\n");   
    printf("\n    <datal> length of the data vectors,\n");
    printf("\n    <memsize> number of data/address vector tupels building the memory,\n");   
    printf("\n    <testsize> number of vectors used to train the address vectors,\n");
    printf("\n    <fillsize> number of vectors stored in the memory,\n");   
    printf("\n    <thres> threshold value and\n");
    printf("\n    <initloops> number of initialization phases to train the address vectors.\n");     
    printf("\n    try for instance '%s 40 50 2000 700 4000 27 7'.", argv[0]);
    printf("\n\n");
    exit(0);
  }
  
  int ADDR_LEN=atoi(argv[1]), DATA_LEN=atoi(argv[2]), MEM_SIZE=atoi(argv[3]), TEST_SIZE=atoi(argv[4]);
  int FILL_SIZE=atoi(argv[5]), THRS=atoi(argv[6]), REPEAT_SIZE=atoi(argv[7]);  
  
  SOSDM sosdm;
  sosdm.create(MEM_SIZE, ADDR_LEN, DATA_LEN, THRS);
  sosdm.create_SO(THRS);
  BinVector v1, v2, b1, b2; 
  KVector hd(MEM_SIZE); // keep all values in the vector
  vei_t d=0;
  
  //create two vectors for input
  v1.create(ADDR_LEN);  v2.create(ADDR_LEN);
  b1.create(DATA_LEN);  b2.create(DATA_LEN);
  for (vei_t i=0;i<ADDR_LEN;i++) { v1.set_comp(0,i);  v2.set_comp(0,i); }   
  for (vei_t i=0;i<DATA_LEN;i++) { b1.set_comp(0,i);  b2.set_comp(0,i); }
  //make one unique for testing reasons
  v1.set_comp(1,2); v1.set_comp(1,11);  v1.set_comp(1,12); v1.tgl_comp(13); 
  v1.tgl_comp(23); v1.tgl_comp(27);   
  b1.set_comp(1,2); b1.set_comp(1,12); b1.set_comp(1,22); b1.set_comp(1,17);

  printf(" Self Organizing Sparse Distributed Memory test.\n\r");

  printf(" Creating %i random vectors as the address space.\n\r", MEM_SIZE);  
  sosdm.random_init(); 
  
  printf(" Storing v1's data vector, which is:\n\r");
  printv(&b1);
  d = sosdm.store_SO(v1, b1);
  sosdm.address_update();
  printf(" in %i locations on address:\n\r",d);
  printv(&v1);
  
  printf(" Adding %i other (random) vectors %i times for initialization  tempsum = new BVector<oas_t>;n random places.\n\r", TEST_SIZE,REPEAT_SIZE); 
  ves_t tm = clock();
  for (vei_t h=0; h<REPEAT_SIZE; h++) 
  { 
  	printf(" Storing v1's data vector, which is:\n\r");
  	printv(&b1);
  	sosdm.del_errovec();
  	d = sosdm.store_SO(v1, b1);
  	sosdm.address_update(); //updating the addresses with the unique address of v1
  	printf(" in %i locations on address:\n\r",d);
  	printv(&v1);
  	for (vei_t k=0; k<TEST_SIZE; k++) 
	{  
     		for (vei_t i=0;i<ADDR_LEN;i++) 
            		v2.set_comp(floor(2.0*rand()/(RAND_MAX+1.0)),i); 
     		for (vei_t i=0;i<DATA_LEN;i++) 
            		b2.set_comp(floor(2.0*rand()/(RAND_MAX+1.0)),i); 
     		d = sosdm.store_SO(v2, b2);
     		if (k<10) 
		{
        		printf(" %i: ",d);
        		for (vei_t i=0;i<v2.get_dim();i++) 
          			printf("%i",v2.get_comp(i));
        		printf("  ");
        		printv(&b2);
     		} 
		else 
			printf(".");     
  	}
  sosdm.address_update(); //updating the addresses with the test vectors 
  sosdm.del_datavec(); //set all elements to zero
  sosdm.del_errovec(); //set all elements to zero
  }	
  tm = clock() - tm;
  printf("time during calculation: %i", tm);   
  printf("\n\r");
  
  //memory gets filld up with final data vectors
  printf(" Adding %i other (random) vectors i   tempsum = new BVector<oas_t>;n random places.\n\r", FILL_SIZE); 
  tm = clock();
  for (vei_t k=0; k<FILL_SIZE; k++) 
  {  
    	for (vei_t i=0;i<ADDR_LEN;i++) 
            	v2.set_comp(floor(2.0*rand()/(RAND_MAX+1.0)),i); 
     	for (vei_t i=0;i<DATA_LEN;i++) 
            	b2.set_comp(floor(2.0*rand()/(RAND_MAX+1.0)),i); 
     	d = sosdm.store_SO(v2, b2);
     	if (k<10) 
	{
        	printf(" %i: ",d);
        	for (vei_t i=0;i<v2.get_dim();i++) 
          		printf("%i",v2.get_comp(i));
        	printf("  ");
        	printv(&b2);
     	} 
	else 
		printf(".");     
  }	
  tm = clock() - tm;
  printf("time during calculation: %i", tm);   
  printf("\n\r");
  
  //store the unique vectors data three times
  printf(" Storing v1's data vector, which is:\n\r");
  printv(&b1);
  sosdm.store_SO(v1, b1);
  sosdm.store_SO(v1, b1);
  d = sosdm.store_SO(v1, b1);
  sosdm.destroy_errvec(); //delete error vector to reduce used memory space
  printf(" in %i locations on address:\n\r",d);	void destruct_mem();
  printv(&v1);
  
  //test how precise the memory works
  printf(" Adding the winner's counters from retrieval of address.\n\r");
  BVector<s_16b> tsum2(b1.get_dim());
  BVector<oas_t> tsum3(b1.get_dim());
  BinVector sum;
  sum.create(DATA_LEN);
  d = sosdm.retrieve_SO(v1,sum,tsum3);
  printf("retrieved from %i locations\n\r",d);
  printf("Which sums up to:\n\r");
  printv3(&tsum3);  printf("\n\r");
  printf("And results in:\n\r");
  printv(&sum);
  printf("Which should match:\n\r");
  printv(&b1);
  
  printf("\n\rDone.\n\r");
  return 0;
}
