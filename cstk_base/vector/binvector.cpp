/***************************************************************************
                            binvector.cpp  -  v.1.10
                            ------------------------
    begin                : Aug 17 2002
    copyright            : (C) 2002-2003 by Kristof Van Laerhoven
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

#include "binvector.h"
 
BinVector::BinVector() 
{
     vect = NULL;
     strout = NULL;
}

BinVector::BinVector(vei_t nsize) 
{
     strout = NULL;
     create(nsize);
}

BinVector::~BinVector() 
{
     if (vect!=NULL) delete []vect;
     if (strout!=NULL) delete []strout;
}

void BinVector::create(vei_t nsize)
{
     size=nsize;
     vect = new u_8b[(size/8)+1];
     xorvec = new u_8b[(size/8)+1];
}

bool BinVector::get_comp(vei_t itr)
{
     return (vect[itr/8] & bmask(itr));
}

bool BinVector::get_xor_comp(vei_t itr)
{
     return (xorvec[itr/8] & bmask(itr));
}

void BinVector::set_comp(bool val, vei_t itr)
{
     val? vect[itr/8]|=bmask(itr): vect[itr/8]&=~bmask(itr);
}

void BinVector::set_xor_comp(bool val, vei_t itr)
{
     val? xorvec[itr/8]|=bmask(itr): xorvec[itr/8]&=~bmask(itr);
}

void BinVector::tgl_comp(vei_t itr) 
{
     vect[itr/8] ^= bmask(itr);
}

vei_t BinVector::get_dim() 
{
     return size;
} 

u_8b BinVector::bmask(vei_t iter)
{
     return (0x01<<(7-(iter%8)));
}

vei_t BinVector::dis_ham(BinVector& invec) 
{
  vei_t dist=0;
  for (vei_t i=0; i<size; i++) {
    if (get_comp(i)!=invec.get_comp(i)) dist++; 
  }
  return dist;
}

vei_t BinVector::dis_nand(BinVector& invec) 
{
  vei_t dist=0;
  for (vei_t i=0; i<size; i++) {
    if (!((get_comp(i) == invec.get_comp(i)) && (get_comp(i) == 1)))
        dist++; 
  }
  return dist;
}

char* BinVector::to_string(void)
{
     if (strout!=NULL) delete []strout;
     strout = new char[size];
     for (vei_t i=0; i<size; i++) {
        if (get_comp(i)==1)
               strout[i] = '1';
        else   strout[i] = '0';
     }
     strout[size] = '\0';
     return strout;
}
