/***************************************************************************
                              sdma.cpp - v.1.00
                             -------------------
    begin                : Jun 01 2004
    copyright            : (C) 2002-2004 by Kristof Van Laerhoven
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

#include "sdma.h" 

SDMA::SDMA() 
{
   av = NULL;
   dvd = NULL;
}

SDMA::SDMA(vei_t nsize, vei_t nasize, vei_t ndsize, oas_t thp) 
{
   size = nsize;
   asize = nasize;
   dsize = ndsize;
   thresholdp = thp;
   av = new BinVector[size];
   dvd = new BVector<oas_t>[size];
}

SDMA::~SDMA() 
{
  if (av!=NULL) delete []av;
  if (dvd!=NULL) delete []dvd;
}

void SDMA::create(vei_t nsize, vei_t nasize, vei_t ndsize, oas_t thp) 
{
   size = nsize;
   asize = nasize;
   dsize = ndsize;
   thresholdp = thp;
   av = new BinVector[size];
   dvd = new BVector<oas_t>[size];
}

void SDMA::random_init() 
{
  for (vei_t j=0;j<size;j++) {
    av[j].create(asize);
    dvd[j].create(dsize);
    for (vei_t i=0;i<asize;i++) {
         dvd[j].set_comp(0,i);
         av[j].set_comp(floor(2.0*rand()/(RAND_MAX+1.0)),i); 
    }
  }
}

vei_t SDMA::retrieve(BinVector& v1, BinVector& tsum, BVector<oas_t>& tempsum, bool det_radius) 
{ 
  vei_t c=0,h=0;
  tmp = 0.0;
  if (det_radius) h = radius(v1);
  for (vei_t j=0;j<size;j++) {
  	if (det_radius)
  		tmp = (1-(thresholdp * (double)(v1.dis_ham(av[j])-h)));
	else
		tmp = (1-(thresholdp * (double)(v1.dis_ham(av[j]))));
    	if (tmp>0) 
	{
        	for (vei_t i=0;i<dsize;i++) 
		{
	  	tempsum.set_comp( tempsum.get_comp(i) + (dvd[j].get_comp(i)*tmp), i ); 
		}
	c++;
    	}    
  }
  for (vei_t i=0;i<dsize;i++) {
     if (tempsum.get_comp(i)>=0) 
        tsum.set_comp(1,i);
     else           
        tsum.set_comp(0,i);
  }  
  return c;
}


vei_t SDMA::remove(oas_t usage)
{
	vei_t h, count=0;
	av_tmp = new BVector<oas_t>[size];
	for (vei_t j=0; j<size; j++)
	{
		av_tmp[j].create(asize);
		h=0;
		for (vei_t i=0; i<dsize; i++)
		{
			if ((dvd[j].get_comp(i)>=-usage) && (dvd[j].get_comp(i)<=usage)) h++;
		}
		if (h!=dsize)
		{
			for (vei_t k=0; k<asize; k++)
			{
				av_tmp[count].set_comp(av[j].get_comp(k),k);
			}
			count++;
		}
	}
	delete []dvd;
	delete []av;
	size = count;
	av = new BinVector[size];
   	dvd = new BVector<oas_t>[size];
	for (vei_t j=0; j<size; j++)
	{
		av[j].create(asize);
		dvd[j].create(dsize);
		for (vei_t k=0; k<asize; k++)
		{
			av[j].set_comp(av_tmp[j].get_comp(k),k);	
		}
		for (vei_t k=0; k<dsize; k++)
		{
			dvd[j].set_comp(0,k);
		}
	}
	delete []av_tmp;
	return size;
}

vei_t SDMA::radius(BinVector& v1)
{
	vei_t hold = 100;
	for (vei_t i=0; i<size; i++)
	{
		if (v1.dis_ham(av[i])<hold)
			hold = v1.dis_ham(av[i]);
	}
	return hold;
}

vei_t SDMA::store(BinVector& v1, BinVector& b1, bool det_radius) 
{ 
	vei_t c=0, h=0;
  	tmp=0.0;
	if (det_radius) h = radius(v1);
  	for (vei_t j=0;j<size;j++) 
  	{
		if (det_radius)
  			tmp = (1-(thresholdp * (double)(v1.dis_ham(av[j])-h)));
		else
			tmp = (1-(thresholdp * (double)(v1.dis_ham(av[j]))));	
  		if (tmp>0.0)
		{
 			for (vei_t i=0;i<dsize;i++) 
			{
	  			if (b1.get_comp(i)==1)
	       				dvd[j].set_comp((dvd[j].get_comp(i)+tmp),i);
	  			else 
	       				dvd[j].set_comp((dvd[j].get_comp(i)-tmp),i);
			}
		c++;
		}
  	}    
  	return c;
}
