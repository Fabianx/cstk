/***************************************************************************
                              sosdm.cpp - v.1.00
                             -------------------
    begin                : Sept 20 2004
    copyright            : (C) 2002-2004 by Martin Berchtold
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

#include "sosdm.h" 

SOSDM::SOSDM()
{
	errv = NULL;
}

void SOSDM::create_SO(vei_t th)
{
	par.threshold = th;
	errv = new BVector<oas_t>[par.size];
	for (vei_t j=0;j<par.size;j++) 
	{
		errv[j].create(par.asize);
		for (vei_t i=0;i<par.asize;i++) 
		{
         		errv[j].set_comp(0,i);
    		}
	}
}

SOSDM::~SOSDM()
{
	if (errv!=NULL) delete []errv;
}

void SOSDM::destroy_errvec()
{
	if (errv!=NULL) delete []errv;
	errv=NULL;
}

vei_t SOSDM::max_invdist(BinVector& v1)
{
	vei_t tmp=0;
	for (vei_t i=0; i<par.size; i++)
	{
		if ((par.asize-v1.dis_ham(av[i]))>tmp) 
			tmp=(par.asize-v1.dis_ham(av[i]));
	}
	return tmp;
}

oas_t SOSDM::signal_thres(BinVector& v1, BinVector& v2, vei_t maxdis)
{
	if ((par.asize-v1.dis_ham(v2))>par.threshold) 
	{
		return ((double(par.asize-v1.dis_ham(v2)))/double(maxdis));
	}
	else
	{
		return 0.0;
	}
}

vei_t SOSDM::store_SO(BinVector& v1, BinVector& b1) 
{ 
	vei_t c=0;
  	vei_t max;
	max=max_invdist(v1);
	oas_t valu;
	total_sig=0.0;
  	for (vei_t j=0;j<par.size;j++) 
  	{
		valu = signal_thres(v1, av[j], max);
  		if (valu>0)
		{
 			for (vei_t i=0;i<par.dsize;i++) 
			{
	  			if (b1.get_comp(i)==1)
	       				dvd[j].set_comp((dvd[j].get_comp(i)+valu),i);
	  			else 
	       				dvd[j].set_comp((dvd[j].get_comp(i)-valu),i);
			}
			total_sig += valu;
			c++;
			error_calc(v1, av[j], valu, j);
		}
  	}    
  	return c;
}

void SOSDM::error_calc(BinVector& v1, BinVector& v2, oas_t sigthr, vei_t counter)
{
	oas_t tmp;
	for (vei_t i=0; i<par.asize; i++)
	{
		tmp = (errv[counter].get_comp(i)+(sigthr*(v1.get_comp(i)-v2.get_comp(i))));
		errv[counter].set_comp((tmp/sigthr),i);	
	}
}

void SOSDM::address_update()
{
	for (vei_t j=0; j<par.size; j++)
	{
		for (vei_t i=0; i<par.asize; i++)
		{
			if (errv[j].get_comp(i)>0)
				av[j].set_comp(1,i);
			else if (errv[j].get_comp(i)<0)
				av[j].set_comp(0,i);
		}	
	}
}

void SOSDM::del_datavec()
{
	for (vei_t k=0;k<par.size;k++) 
	{
		for (vei_t i=0;i<par.asize;i++) 
		{
         		dvd[k].set_comp(0,i);
    		}
	}
}

void SOSDM::del_errovec()
{
	for (vei_t k=0;k<par.size;k++) 
	{
		for (vei_t i=0;i<par.asize;i++) 
		{
         		errv[k].set_comp(0,i);
    		}
	}
}

vei_t SOSDM::retrieve_SO(BinVector& v1, BinVector& tsum, BVector<oas_t>& tempsum)
{
	vei_t c=0;
	vei_t max=max_invdist(v1);
	oas_t tmp = 0.0;
	for (vei_t j=0;j<par.size;j++) {
  		tmp = signal_thres(v1, av[j], max);
    		if (tmp>0) 
		{
        		for (vei_t i=0;i<par.dsize;i++) 
			{
				tempsum.set_comp( tempsum.get_comp(i) + (dvd[j].get_comp(i)*tmp), i ); 
			}
		c++;
    		}    
	}
	for (vei_t i=0;i<par.dsize;i++) 
	{
			if (tempsum.get_comp(i)>0) 
        			tsum.set_comp(1,i);
			else           
				tsum.set_comp(0,i);
	}  
	return c;
}








