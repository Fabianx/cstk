/***************************************************************************
                              sdmfct.cpp - v.1.00
                             ---------------------
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

#include "sdmafct.h" 

SDMAfct::SDMAfct() 
{
   //av = NULL;
   //dvd = NULL;
}

SDMAfct::SDMAfct(vei_t nsize, vei_t nasize, vei_t ndsize) 
{
   //size = nsize;
   //asize = nasize;
   //dsize = ndsize;
   //av = new BinVector[size];
   //dvd = new BVector<oas_t>[size];
}

SDMAfct::~SDMAfct() 
{
  //if (av!=NULL) delete []av;
  //if (dvd!=NULL) delete []dvd;
}


vei_t SDMAfct::store_gauss(BinVector& v1, BinVector& b1, bool det_radius, oas_t density) 
{ 
	vei_t c=0, h=0;
  	tmp=0.0;
	if (det_radius) h = radius(v1);
  	for (vei_t j=0;j<size;j++) 
  	{
		if (det_radius)
  			tmp = ((1/(density*sqrt(2*pi)))*pow(e,-(pow((double)(v1.dis_ham(av[j])-h),2)/(2*density*density))));
		else
			tmp = ((1/(density*sqrt(2*pi)))*pow(e,-(pow((double)(v1.dis_ham(av[j])),2)/(2*density*density))));	
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

vei_t SDMAfct::store_nand(BinVector& v1, BinVector& b1, bool det_radius) 
{ 
	vei_t c=0, h=0;
  	tmp=0.0;
	if (det_radius) h = radius_nand(v1);
  	for (vei_t j=0;j<size;j++) 
  	{
		if (det_radius)
  			tmp = (1-(thresholdp * (double)(v1.dis_nand(av[j])-h)));
		else
			tmp = (1-(thresholdp * (double)(v1.dis_nand(av[j]))));	
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

vei_t SDMAfct::retrieve_gauss(BinVector& v1, BinVector& tsum, BVector<oas_t>& tempsum, bool det_radius, oas_t density) 
{ 
  vei_t c=0,h=0;
  tmp = 0.0;
  if (det_radius) h = radius(v1);
  for (vei_t j=0;j<size;j++) {
  	if (det_radius)
  		tmp = ((1/(density*sqrt(2*pi)))*pow(e,-(pow((double)(v1.dis_ham(av[j])-h),2)/(2*density*density))));
	else
		tmp = ((1/(density*sqrt(2*pi)))*pow(e,-(pow((double)(v1.dis_ham(av[j])),2)/(2*density*density))));
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

vei_t SDMAfct::retrieve_nand(BinVector& v1, BinVector& tsum, BVector<oas_t>& tempsum, bool det_radius) 
{ 
  vei_t c=0,h=0;
  tmp = 0.0;
  if (det_radius) h = radius_nand(v1);
  for (vei_t j=0;j<size;j++) {
  	if (det_radius)
  		tmp = (1-(thresholdp * (double)(v1.dis_nand(av[j])-h)));
	else
		tmp = (1-(thresholdp * (double)(v1.dis_nand(av[j]))));
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

vei_t SDMAfct::radius_nand(BinVector& v1)
{
	vei_t hold = 100;
	for (vei_t i=0; i<size; i++)
	{
		if (v1.dis_nand(av[i])<hold)
			hold = v1.dis_ham(av[i]);
	}
	return hold;
}

