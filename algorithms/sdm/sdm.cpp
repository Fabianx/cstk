/***************************************************************************
                              sdm.cpp - v.1.00
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

#include "sdm.h" 

SDM::SDM() 
{
   av = NULL;
   dv = NULL;
}

SDM::SDM(vei_t nsize, vei_t nasize, vei_t ndsize, vei_t th) 
{
   size = nsize;
   asize = nasize;
   dsize = ndsize;
   threshold = th;
   av = new BinVector[size];
   dv = new BVector<s_16b>[size];
}

SDM::~SDM() 
{
  if (av!=NULL) delete []av;
  if (dv!=NULL) delete []dv;
}

void SDM::create(vei_t nsize, vei_t nasize, vei_t ndsize, vei_t th) 
{
   size = nsize;
   asize = nasize;
   dsize = ndsize;
   threshold = th;
   av = new BinVector[size];
   dv = new BVector<s_16b>[size];
}

void SDM::random_init() 
{
  for (vei_t j=0;j<size;j++) {
    av[j].create(asize);
    dv[j].create(dsize);
    for (vei_t i=0;i<asize;i++) {
         dv[j].set_comp(0,i);
         av[j].set_comp(floor(2.0*rand()/(RAND_MAX+1.0)),i); 
    }
  }
}

vei_t SDM::store(BinVector& v1, BinVector& b1, vei_t th) 
{ 
  vei_t c=0;
  for (vei_t j=0;j<size;j++) {
    if ((v1.dis_ham(av[j]))<th) {
        for (vei_t i=0;i<dsize;i++) {
	  if (b1.get_comp(i)==1)
	       dv[j].inc_comp(i);
	  else 
	       dv[j].dec_comp(i);
	}
	c++;
    }    
  }
  return c;
}

vei_t SDM::store(BinVector& v1, BinVector& b1) 
{
  vei_t i=0, c=0;
  // determine the right threshold radius automatically:
  while ( (c<threshold) && (i<max_vei) ){
      c = 0;
      for (vei_t j=0;j<size;j++) {
        if (v1.dis_ham(av[j])<i)  c++;
      }
      i++;
  }
  // store the pattern in the appropriate radius:
  c = store(v1,b1,i-1);
  return c;
}

vei_t SDM::retrieve(BinVector& v1, BVector<s_16b>& tsum, vei_t th) 
{ 
  vei_t c=0;
  for (vei_t j=0;j<size;j++) {
    if (v1.dis_ham(av[j])<th) {
        for (vei_t i=0;i<dsize;i++) {
	  tsum.set_comp( tsum.get_comp(i) + dv[j].get_comp(i), i ); 
	}
	c++;
    }    
  }
  return c;
}

vei_t SDM::retrieve(BinVector& v1, BinVector& tsum, BVector<s_16b>& tempsum) 
{ 
  vei_t i=0, c=0;
  // determine the right threshold radius automatically:
  while ( (c<threshold) && (i<max_vei) ){
      c = 0;
      for (vei_t j=0;j<size;j++) {
        if (v1.dis_ham(av[j])<i)  c++;
      }
      i++;
  }
  // do the retrieving of the sums:
  c = retrieve(v1, tempsum, i-1);
  // extract the pattern:
  for (vei_t i=0;i<dsize;i++) {
     if (tempsum.get_comp(i)>=0) 
        tsum.set_comp(1,i);
     else           
        tsum.set_comp(0,i);
  }  
  return c;
}

